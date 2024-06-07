# GetStarted


# Screen Shot without Debug Mode
```
I (327) uart_rx_task: Start
I (327) uart_tx_task: Start
I (1397) MAIN: DF_begin=1
I (1397) MAIN: DFPlayer Mini online.
I (1397) MAIN: Play first track on 01 folder.
Number:1 Play Finished!
Number:1 Play Finished!
```


# Screen Shot with Debug Mode
```
I (327) uart_rx_task: Start
I (327) uart_tx_task: Start

sending:7e ff 6 c 1 0 0 fe ee ef
I (467) uart_rx_task: Read 10 bytes
I (467) uart_rx_task: data[0]=0x7e
I (467) uart_rx_task: data[1]=0xff
I (467) uart_rx_task: data[2]=0x6
I (467) uart_rx_task: data[3]=0x41
I (467) uart_rx_task: data[4]=0x0
I (477) uart_rx_task: data[5]=0x0
I (477) uart_rx_task: data[6]=0x0
I (487) uart_rx_task: data[7]=0xfe
I (487) uart_rx_task: data[8]=0xba
I (487) uart_rx_task: data[9]=0xef
received:7e ff 6 41 0 0 0 fe ba ef
I (507) uart_rx_task: Read 1 bytes
I (507) uart_rx_task: data[0]=0xff
received:ff I (1197) uart_rx_task: Read 10 bytes
I (1197) uart_rx_task: data[0]=0x7e
I (1197) uart_rx_task: data[1]=0xff
I (1197) uart_rx_task: data[2]=0x6
I (1197) uart_rx_task: data[3]=0x3f
I (1207) uart_rx_task: data[4]=0x0
I (1207) uart_rx_task: data[5]=0x0
I (1207) uart_rx_task: data[6]=0x2
I (1217) uart_rx_task: data[7]=0xfe
I (1217) uart_rx_task: data[8]=0xba
I (1227) uart_rx_task: data[9]=0xef
received:7e ff 6 3f 0 0 2 fe ba ef
I (1427) MAIN: DF_begin=1
I (1427) MAIN: DFPlayer Mini online.
I (1427) MAIN: Play first track on 01 folder.

sending:7e ff 6 6 1 0 1e fe d6 ef
I (1477) uart_rx_task: Read 10 bytes
I (1477) uart_rx_task: data[0]=0x7e
I (1477) uart_rx_task: data[1]=0xff
I (1477) uart_rx_task: data[2]=0x6
I (1477) uart_rx_task: data[3]=0x41
I (1477) uart_rx_task: data[4]=0x0
I (1487) uart_rx_task: data[5]=0x0
I (1487) uart_rx_task: data[6]=0x0
I (1497) uart_rx_task: data[7]=0xfe
I (1497) uart_rx_task: data[8]=0xba
I (1507) uart_rx_task: data[9]=0xef
received:7e ff 6 41 0 0 0 fe ba ef

sending:7e ff 6 3 1 0 1 fe f6 ef
I (1557) uart_rx_task: Read 10 bytes
I (1557) uart_rx_task: data[0]=0x7e
I (1557) uart_rx_task: data[1]=0xff
I (1557) uart_rx_task: data[2]=0x6
I (1557) uart_rx_task: data[3]=0x41
I (1557) uart_rx_task: data[4]=0x0
I (1567) uart_rx_task: data[5]=0x0
I (1567) uart_rx_task: data[6]=0x0
I (1577) uart_rx_task: data[7]=0xfe
I (1577) uart_rx_task: data[8]=0xba
I (1587) uart_rx_task: data[9]=0xef
received:7e ff 6 41 0 0 0 fe ba ef
I (32907) uart_rx_task: Read 20 bytes
I (32907) uart_rx_task: data[0]=0x7e
I (32907) uart_rx_task: data[1]=0xff
I (32907) uart_rx_task: data[2]=0x6
I (32907) uart_rx_task: data[3]=0x3d
I (32917) uart_rx_task: data[4]=0x0
I (32917) uart_rx_task: data[5]=0x0
I (32917) uart_rx_task: data[6]=0x1
I (32927) uart_rx_task: data[7]=0xfe
I (32927) uart_rx_task: data[8]=0xbd
I (32937) uart_rx_task: data[9]=0xef
I (32937) uart_rx_task: data[10]=0x7e
I (32937) uart_rx_task: data[11]=0xff
I (32947) uart_rx_task: data[12]=0x6
I (32947) uart_rx_task: data[13]=0x3d
I (32957) uart_rx_task: data[14]=0x0
I (32957) uart_rx_task: data[15]=0x0
I (32967) uart_rx_task: data[16]=0x1
I (32967) uart_rx_task: data[17]=0xfe
I (32967) uart_rx_task: data[18]=0xbd
I (32977) uart_rx_task: data[19]=0xef
received:7e ff 6 3d 0 0 1 fe bd ef
Number:1 Play Finished!
received:7e ff 6 3d 0 0 1 fe bd ef
Number:1 Play Finished!
```

