set(component_srcs "serial.c" "DFRobotDFPlayerMini.c")

idf_component_register(SRCS "${component_srcs}"
                       PRIV_REQUIRES driver
                       INCLUDE_DIRS ".")
