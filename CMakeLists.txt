include_directories(./include)
add_executable(pcapc ./main.c ./dpi/dpi_ether.c ./dpi/dpi_ip.c ./dpi/dpi_tcp.c ./dpi/dpi_udp.c)
target_link_libraries(pcapc pcap)
