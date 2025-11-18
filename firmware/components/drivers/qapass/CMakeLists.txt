idf_component_register(SRCS "qp_core.c" "qp_module.c"
                    INCLUDE_DIRS "include"
		    REQUIRES freertos driver format
		)
