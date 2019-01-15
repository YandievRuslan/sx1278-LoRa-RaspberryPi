<p># sx1278-LoRa-RaspberryPi</p>
<h1>Library to work with sx1278 LoRa chips on Raspberry pi</h1>
<h2>It's just for P2P transmissions. Not for LoRaWAN.</h2>
<p><br /> May work with all sx127x chips, i didn't test it on these chips</p>
<p>This source uses libpigpio. Installation on raspbian: <strong><em>apt install pigpio</em></strong>&nbsp;</p>
<p><strong><em>Quick start:</em></strong></p>
<blockquote>
<p>0. <em>apt install pigpio</em></p>
<p>1. Wire raspberry and lora chip by the table below</p>
<table style="height: 170px;" width="232">
<tbody>
<tr>
<td style="width: 70px;">raspi</td>
<td style="width: 70px;">&nbsp;</td>
<td style="width: 70px;">LoRa</td>
</tr>
<tr>
<td style="width: 70px;">GPIO4</td>
<td style="width: 70px;">&nbsp;</td>
<td style="width: 70px;">RESET</td>
</tr>
<tr>
<td style="width: 70px;">GPIO17</td>
<td style="width: 70px;">&nbsp;</td>
<td style="width: 70px;">DIO0</td>
</tr>
<tr>
<td style="width: 70px;">MOSI</td>
<td style="width: 70px;">&nbsp;</td>
<td style="width: 70px;">MOSI</td>
</tr>
<tr>
<td style="width: 70px;">MISO</td>
<td style="width: 70px;">&nbsp;</td>
<td style="width: 70px;">MISO</td>
</tr>
<tr>
<td style="width: 70px;">CLK</td>
<td style="width: 70px;">&nbsp;</td>
<td style="width: 70px;">SCK</td>
</tr>
<tr>
<td style="width: 70px;">SPI_CE0</td>
<td style="width: 70px;">&nbsp;</td>
<td style="width: 70px;">NSS</td>
</tr>
</tbody>
</table>
<p>2. Clone the repo</p>
<p>3. Enter cloned repo dir</p>
<p>4. <em>make</em></p>
<p>5. Try files under sudo: <em>transmit_explicit, transmit_implicit, receive_explicit, receive_implicit</em></p>
<p>All transmitt examples just ones send "LoRa" string with its '\0' terminating byte.</p>
</blockquote>
<h3><strong>API:</strong></h3>
<blockquote>
<p><strong>int LoRa_begin(LoRa_ctl *modem) -</strong> Configures radio module. Returns spi handle &gt;= 0 if OK, otherwise error number &lt; 0</p>
<p><br /><strong>void LoRa_send(LoRa_ctl *modem) - </strong>Commands radio module to send&nbsp; data from buffer</p>
<p><br /><strong>void LoRa_receive(LoRa_ctl *modem) - </strong>Commands radio module to switch to continuous receive mode</p>
<p><strong>void LoRa_calculate_packet_t(LoRa_ctl *modem) - </strong>Calculates onair time, payload symbols and other fields.</p>
<p><strong>_Bool LoRa_check_conn(LoRa_ctl *modem) - </strong>Checks connection vith module. Return True/False</p>
<p><strong>void LoRa_end(LoRa_ctl *modem) - </strong>Ends up working with module. Switches to sleep mode, closes spi and so on.</p>
<p><strong>void LoRa_stop_receive(LoRa_ctl *modem) -&nbsp;</strong>Removes callback from IRQ pin, switches module to idle mode.</p>
<p><strong>void LoRa_sleep(LoRa_ctl *modem) - </strong>Switches module to sleep mode</p>
<p><strong>unsigned char LoRa_get_op_mode(LoRa_ctl *modem)</strong> -<strong>&nbsp;</strong>Returns current module operational mode - SLEEP_MODE, RXCONT_MODE, SLEEP_MODE, STDBY_MODE</p>
<p><strong>struct&nbsp;LoRa_ctl - </strong>view LoRa.h</p>
</blockquote>
<h3><strong>Notice:</strong></h3>
<blockquote>
<p><span class="pl-k">Function </span><em><span style="color: #0000ff;"> <span class="pl-en">void LoRa_receive(LoRa_ctl *modem)</span></span></em> uses continuous mode on module. Radio module will continiuously receive packets and each time execute user callback. It's in case you need to exchange data larger than 255 bytes(FIFO buffer). You have to manually stop receiving by&nbsp;<span style="color: #0000ff;"><em><span class="pl-k">void</span> <span class="pl-en">LoRa_stop_receive</span>(LoRa_ctl *modem)</em>.</span></p>
</blockquote>
<h3><strong>15.01.2019 update</strong></h3>
<blockquote>
<p>Updated library to handle radio module from user Rx\Tx callbacks. Backward compatibility retained.</p>
<p>Now you can start sending data from Rx callback, or vice versa, or any other operation with radio module.</p>
<p>New examples "ping.c" and "pong.c" to explain new features. Make, then execute on different devices first "pong" then "ping".</p>
</blockquote>
<h2><strong>Supported by <a href="http://zaoitt.ru/">ITT ltd</a>&nbsp;</strong></h2>
