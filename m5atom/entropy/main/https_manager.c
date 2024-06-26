/*
 * https_manager.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Pollock
 */


#include "https_manager.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "tcpip_adapter.h"
#include "freertos/queue.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "esp_http_client.h"
#include "esp_tls.h"



/* NOTE:
 * For OAUTH2 token to work properly, change the variable
 * DEFAULT_HTTP_BUF_SIZE (512) to DEFAULT_HTTP_BUF_SIZE (2048)
 * in "esp_http_client.h" file which can be found in
 * esp-idf\components\esp_http_client\include
 */

#define HTTPS_DEBUG

static const char *TAG = "HTTPS_MANAGER";


//temp
static const char *bearer_token = "Bearer eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiIsImp0aSI6IjllZmU0MTY0NWY0Y2VmMmM1ZjY3MjI5ZGMzNzA5MDczYTUwZDBiMjgyNmM5MzBiNDIyZjFkYjY1YmI3Njc1ZWNlOTAyNmMxNGYwMjhhNDdmIn0.eyJhdWQiOiIyIiwianRpIjoiOWVmZTQxNjQ1ZjRjZWYyYzVmNjcyMjlkYzM3MDkwNzNhNTBkMGIyODI2YzkzMGI0MjJmMWRiNjViYjc2NzVlY2U5MDI2YzE0ZjAyOGE0N2YiLCJpYXQiOjE2MTM0OTg0MDAsIm5iZiI6MTYxMzQ5ODQwMCwiZXhwIjoxNjQ1MDM0NDAwLCJzdWIiOiIxX3Rlcm1pbmFsIiwic2NvcGVzIjpbIioiXX0.qKar9cVH2oasJmI5xCqFsdMMGkAH-PCcEK2zYtdJycJbcUHU2HghUxhwnln0QDOy1lvSVHheLReaLL8GL30FJe72b3YeBqTn8z-oSsaY-nCBsmwugHlYPWY4jpkDFeX4Z0X6UqOEczB25XMjX22c94loIyaa6fO_WB7JX9QWoi0941BvCwqRqZmdcDRjrHmQBtfRLDS0QiuaO3F6QgxLh792L0_wwXEzMYp0uTj-iVtjyMmYbbyoqyWOi9S1PXR9_LYjvLUyacGXMxCbk0dowF55ww46AuNge6jcyLEeKugp6WmZvsL9YcZWIUGdoi-j3BV2OZSGqeqwro5HfTBppoCOwp5U9_qGGjuyAtlS_FHLKw8lYckDdrtE9jJv2mUtjsiwE2lfYQJaBsje7njKSGLFagMYXuGEtmAUVL1Yw5Hp5YNpZOdUjCL2KopheEauFiycsi6QRkGfKp_ffXufoCZ8dzp_L_kPeKfw4-TgdlQTPPFNRJ6_sAB-NjbTZ8rfT7l0MSIX_xC-kvebrEpx9Z8J_mUdEMgskfwgg7PImXvQj6spvfLC-LMl1n1020443OI54vvs7bH-hV7nnkEM4_A3HwHAGsolrlUA8h9qYVGf0U4LqRiKIqPd_N62j1H8ZhAZO87JR8OEXYRVeh08n7JUlKSiTJHEzPou4dg5rYo";

void https_post_data(char *_postdata)
{
#ifdef HTTPS_DEBUG
	    ESP_LOGI(TAG, "GETTING OAUTH BEARER TOKEN\r\n");
#endif

		//_postdata = "{\"temperature\": 36.6,\"person\": \"LSNAOJF\"}";// = json_oauth_credentials_body_generator(&c1);

//"{\"temperature\": 37.5,\"person\": \"LSNAOJF\"}"

	char *https_request_buffer = malloc((BEARER_BUFFER_SIZE)*sizeof(char));
	//maybe return successful or not
    esp_http_client_config_t config = {
		.host = DF_HOST,
		.path = OAUTH_PATH,
		.transport_type = HTTP_TRANSPORT_OVER_SSL,

    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err ;
    esp_http_client_set_method(client, HTTP_METHOD_POST);
    esp_http_client_set_post_field(client, _postdata, strlen(_postdata));
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_header(client, "Authorization", bearer_token);

    err = esp_http_client_open(client, strlen(_postdata));
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
            esp_http_client_cleanup(client);
           //task_fatal_error(); //#TODO what does this do-> Triggers reset
        }
    	esp_http_client_write(client, _postdata, strlen(_postdata));
        esp_http_client_fetch_headers(client);

#ifdef HTTPS_DEBUG
        ESP_LOGI(TAG, "HTTPS Status = %d, content_length = %d",  esp_http_client_get_status_code(client),    esp_http_client_get_content_length(client));
#endif
    int data_read = esp_http_client_read(client, https_request_buffer, BEARER_BUFFER_SIZE);

    if (data_read < 0)
    {
		ESP_LOGE(TAG, "Error: SSL data read error");

                //#TODO Handle Error Check for HTTPS Response 200
    }
    else if (data_read > 0)
    {
#ifdef HTTPS_DEBUG
		 ESP_LOGI(TAG, "POST Request Successful\r\n");
#endif

    }
    free(https_request_buffer);

    esp_http_client_close(client);
    esp_http_client_cleanup(client);


}
