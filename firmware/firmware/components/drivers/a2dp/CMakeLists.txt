idf_component_register(SRCS "bt_app_core.c"
                            "bt_app_module.c"
                    INCLUDE_DIRS "include"
		    REQUIRES bt freertos nvs_flash driver a2dp mp3_decoder flac_decoder
		)
