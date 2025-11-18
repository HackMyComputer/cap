idf_component_register(SRCS "hw_uart.c"
                    INCLUDE_DIRS "include"
		    REQUIRES 
		    	freertos
			driver
		)
