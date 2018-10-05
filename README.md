This is a project rewrite in nodejs. Source is from  https://github.com/function86437/scratch_project_nodejs

# Installation
* [下載截圖說明](https://hackmd.io/s/B1bSCBzUb)
* [安裝操作教學](https://hackmd.io/s/SyjIY4ZVZ)
* [Multi-WiFiS4A使用說明](https://hackmd.io/s/HJhGPQb_Z)

## Architecture
![](https://i.imgur.com/WfYGQZd.png)

* [Multi-WiFiS4A使用說明](https://hackmd.io/s/HJhGPQb_Z)
## tools
### software
<ul>
	<li><a href="https://nodejs.org/en/">nodejs</a></li>
	<li><a href="http://s4a.cat/#downloads">S4A</a></li>
	<li><a href="https://www.arduino.cc/en/Main/Software">Arduino</a></li>
	<li><a href="">arduino uno</a></li>
</ul>

### harwardware

<ul>
	<li><a href="https://www.arduino.cc/en/Main/ArduinoBoardUno">arduino uno</a></li>
	<li><a href="https://wiki.wemos.cc/products:d1:d1_mini">Wemos D1 mini</a></li>
</ul>

## Problem with Arduino IDE and Wemos-D1-min
<ul>
	<li>Q1: I can't find Wemos-D1-mini in Arduino IDE(Tools > Board)? </li>
		<p>You have to add this url <code>http://arduino.esp8266.com/stable/package_esp8266com_index.json</code> in Arduino > Perferences > Additional Boards Manager URLs, then update the board firmware using board manager(path: Tools > Board > Board Manager > search for ESP266 > install > Done  </p>
</ul>

## step by step

<ol>
	<li>Burn uno.ino to your uno board by using aduino IDE</li>
	<li>Prepare more than two Wemos-D1-mini. Burn wifionuno.ino to one of Wemos-D1-mini, and burn Client.ino to others</li>
	<li>Setup environment of nodejs by <a href="https://nodejs.org/en/download/">tutorial</a></li>
	
</ol>

# Start
<ol>
	<li>Connect Wemos-D1-mini(wifionuno) to UNO board(pin by pin)<br><code>Uno -> D1-mini</code><br> 
								 <code>3  ->   tx  </code> <br>
								 <code>2  ->   rx  </code> <br>
								 <code>5v ->   5v  </code> <br>
								 <code>GND->   G</code></li>
								 
	<li>Import project in Scratch and then press green flagto start the stage.</li>
	<li>Let Scratch listen on UNO. Simulate button pressed by connecting pin of D2 and GND on D1-mini</li>

</ol>
