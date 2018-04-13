# sx1278-LoRa-RaspberryPi
<h1>Library to work with sx1278 LoRa chips on Raspberry pi</h1>
<h2>It's just for P2P transmittions. Not for LoRaWAN.</h2>
<p><br /> May work with all sx127x chips, i didn't test it on these chips</p>
<p>This source uses libpigpio. Installation on raspbian: <strong><em>apt install pigpio</em></strong></p>
<p>&nbsp;</p>
<p><strong><em>Quick start:</em></strong></p>
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
<p>&nbsp;</p>
<h2><strong>TODO:</strong></h2>
<p>I don't sure about my RSSI and SNR calculation. Please fix my code if it's really wrong and you know the solution.</p>
