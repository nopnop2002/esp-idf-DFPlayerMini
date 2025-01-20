set(component_srcs "main.c" "tcp_server.c")

idf_component_register(SRCS "${component_srcs}"
                       INCLUDE_DIRS ".")
