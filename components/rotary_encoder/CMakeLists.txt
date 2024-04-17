set(requires espressif__button espressif__knob)
idf_component_register(
                    SRCS "rotary_encoder.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "${requires}"
                    ) 
