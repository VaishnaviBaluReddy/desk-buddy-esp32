#include "wifi.h"

#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_netif_sntp.h"
#include "esp_wifi.h"
#include "nvs_flash.h"

#include <assert.h>
#include <string.h>
#include <time.h>

// ==================================================
// WIFI SETTINGS
// ==================================================

#define WIFI_SSID       "BK REDDY"
#define WIFI_PASSWORD   "123456789"

// ==================================================
// TAG
// ==================================================

static const char *TAG = "WIFI";

// ==================================================
// WIFI STATE
// ==================================================

static bool wifi_initialized = false;

// ==================================================
// WIFI EVENT HANDLER
// ==================================================

static void wifi_event_handler(
    void *arg,
    esp_event_base_t event_base,
    int32_t event_id,
    void *event_data
)
{
    (void)arg;

    // ==================================================
    // WIFI STARTED
    // ==================================================

    if (
        event_base == WIFI_EVENT &&
        event_id == WIFI_EVENT_STA_START
    )
    {
        ESP_LOGI(
            TAG,
            "Wi-Fi started. Connecting..."
        );

        esp_err_t err =
            esp_wifi_connect();

        if (err != ESP_OK)
        {
            ESP_LOGE(
                TAG,
                "esp_wifi_connect() failed: %s",
                esp_err_to_name(err)
            );
        }

        return;
    }

    // ==================================================
    // WIFI DISCONNECTED
    // ==================================================

    if (
        event_base == WIFI_EVENT &&
        event_id == WIFI_EVENT_STA_DISCONNECTED
    )
    {
        wifi_event_sta_disconnected_t *event =
            static_cast<wifi_event_sta_disconnected_t *>(event_data);

        ESP_LOGW(
            TAG,
            "Wi-Fi disconnected. Reason: %d",
            event->reason
        );

        esp_err_t err =
            esp_wifi_connect();

        if (err != ESP_OK)
        {
            ESP_LOGE(
                TAG,
                "Wi-Fi reconnect failed: %s",
                esp_err_to_name(err)
            );
        }

        return;
    }

    // ==================================================
    // GOT IP ADDRESS
    // ==================================================

    if (
        event_base == IP_EVENT &&
        event_id == IP_EVENT_STA_GOT_IP
    )
    {
        ESP_LOGI(
            TAG,
            "Wi-Fi connected and IP address obtained."
        );

        // --------------------------------------------------
        // Start SNTP
        // --------------------------------------------------

        esp_err_t err =
            esp_netif_sntp_start();

        if (
            err != ESP_OK &&
            err != ESP_ERR_INVALID_STATE
        )
        {
            ESP_LOGE(
                TAG,
                "Failed to start SNTP: %s",
                esp_err_to_name(err)
            );

            return;
        }

        ESP_LOGI(
            TAG,
            "SNTP started. Waiting for time synchronization..."
        );

        return;
    }
}

// ==================================================
// INITIALIZE WIFI
// ==================================================

void wifi_init()
{
    // ==================================================
    // PREVENT DOUBLE INITIALIZATION
    // ==================================================

    if (wifi_initialized)
    {
        ESP_LOGI(
            TAG,
            "Wi-Fi already initialized."
        );

        return;
    }

    wifi_initialized = true;

    // ==================================================
    // NVS
    // ==================================================

    esp_err_t nvs_result =
        nvs_flash_init();

    if (
        nvs_result == ESP_ERR_NVS_NO_FREE_PAGES ||
        nvs_result == ESP_ERR_NVS_NEW_VERSION_FOUND
    )
    {
        ESP_ERROR_CHECK(
            nvs_flash_erase()
        );

        nvs_result =
            nvs_flash_init();
    }

    ESP_ERROR_CHECK(
        nvs_result
    );

    // ==================================================
    // TCP/IP STACK
    // ==================================================

    esp_err_t netif_result =
        esp_netif_init();

    if (
        netif_result != ESP_OK &&
        netif_result != ESP_ERR_INVALID_STATE
    )
    {
        ESP_ERROR_CHECK(
            netif_result
        );
    }

    // ==================================================
    // DEFAULT EVENT LOOP
    // ==================================================

    esp_err_t event_loop_result =
        esp_event_loop_create_default();

    if (
        event_loop_result != ESP_OK &&
        event_loop_result != ESP_ERR_INVALID_STATE
    )
    {
        ESP_ERROR_CHECK(
            event_loop_result
        );
    }

    // ==================================================
    // DEFAULT WIFI STATION
    // ==================================================

    esp_netif_t *wifi_netif =
        esp_netif_create_default_wifi_sta();

    assert(
        wifi_netif != nullptr
    );

    // ==================================================
    // WIFI DRIVER
    // ==================================================

    wifi_init_config_t wifi_driver_config =
        WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK(
        esp_wifi_init(
            &wifi_driver_config
        )
    );

    // ==================================================
    // EVENT HANDLERS
    // ==================================================

    ESP_ERROR_CHECK(
        esp_event_handler_register(
            WIFI_EVENT,
            ESP_EVENT_ANY_ID,
            &wifi_event_handler,
            nullptr
        )
    );

    ESP_ERROR_CHECK(
        esp_event_handler_register(
            IP_EVENT,
            IP_EVENT_STA_GOT_IP,
            &wifi_event_handler,
            nullptr
        )
    );

    // ==================================================
    // WIFI CONFIGURATION
    // ==================================================

    wifi_config_t station_config = {};

    // --------------------------------------------------
    // SSID
    // --------------------------------------------------

    strncpy(
        reinterpret_cast<char *>(
            station_config.sta.ssid
        ),
        WIFI_SSID,
        sizeof(station_config.sta.ssid) - 1
    );

    // --------------------------------------------------
    // PASSWORD
    // --------------------------------------------------

    strncpy(
        reinterpret_cast<char *>(
            station_config.sta.password
        ),
        WIFI_PASSWORD,
        sizeof(station_config.sta.password) - 1
    );

    // --------------------------------------------------
    // Authentication
    // --------------------------------------------------

    station_config.sta.threshold.authmode =
        WIFI_AUTH_WPA2_PSK;

    // ==================================================
    // SET WIFI MODE
    // ==================================================

    ESP_ERROR_CHECK(
        esp_wifi_set_mode(
            WIFI_MODE_STA
        )
    );

    // ==================================================
    // APPLY WIFI CONFIGURATION
    // ==================================================

    ESP_ERROR_CHECK(
        esp_wifi_set_config(
            WIFI_IF_STA,
            &station_config
        )
    );

    // ==================================================
    // TIMEZONE
    // ==================================================
    //
    // India Standard Time:
    // UTC + 5:30
    //
    // This makes localtime_r() return Indian time.
    //
    // ==================================================

    setenv(
        "TZ",
        "IST-5:30",
        1
    );

    tzset();

    // ==================================================
    // SNTP CONFIGURATION
    // ==================================================
    //
    // We initialize SNTP here, but DON'T start it yet.
    //
    // It will be started after Wi-Fi obtains an IP address.
    //
    // ==================================================

    esp_sntp_config_t sntp_config =
        ESP_NETIF_SNTP_DEFAULT_CONFIG(
            "pool.ntp.org"
        );

    sntp_config.start = false;

    ESP_ERROR_CHECK(
        esp_netif_sntp_init(
            &sntp_config
        )
    );

    // ==================================================
    // START WIFI
    // ==================================================

    ESP_ERROR_CHECK(
        esp_wifi_start()
    );

    ESP_LOGI(
        TAG,
        "Wi-Fi initialization complete."
    );
}