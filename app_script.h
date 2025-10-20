#ifndef APP_SCRIPT_H
#define APP_SCRIPT_H

#include <Arduino.h>
#include "languages.h"

inline String buildAppScript() {
  String script;
  script.reserve(14000);
  script += "let currentLang='" + String(currentLanguage == LANG_FR ? "fr" : "en") + "';";

  // Changement de langue
  script += "function changeLang(lang,evt){";
  script += "if(evt&&evt.preventDefault)evt.preventDefault();";
  script += "fetch('/api/set-language?lang='+lang).then(r=>r.json()).then(d=>{";
  script += "if(d.success){currentLang=lang;";
  script += "document.querySelectorAll('.lang-btn').forEach(b=>b.classList.remove('active'));";
  script += "if(evt&&evt.target){evt.target.classList.add('active');}";
  script += "window.location.reload();";
  script += "}else{console.error('Lang switch refused');}";
  script += "}).catch(e=>console.error('Lang switch error',e));";
  script += "}";

  // Navigation onglets
  script += "function showTab(t,evt){";
  script += "document.querySelectorAll('.tab-content').forEach(e=>e.classList.remove('active'));";
  script += "document.querySelectorAll('.nav-btn').forEach(e=>e.classList.remove('active'));";
  script += "document.getElementById(t).classList.add('active');";
  script += "if(evt&&evt.target){evt.target.classList.add('active');}";
  script += "if(t==='wireless'){refreshWirelessInfo();}";
  script += "}";

  // LED intégrée
  script += "function configBuiltinLED(){fetch('/api/builtin-led-config?gpio='+document.getElementById('ledGPIO').value)";
  script += ".then(r=>r.json()).then(d=>{document.getElementById('builtin-led-status').innerHTML=d.message})}";
  script += "function testBuiltinLED(){document.getElementById('builtin-led-status').innerHTML='" + String(T().testing) + "';";
  script += "fetch('/api/builtin-led-test').then(r=>r.json()).then(d=>{document.getElementById('builtin-led-status').innerHTML=d.result})}";
  script += "function ledBlink(){fetch('/api/builtin-led-control?action=blink').then(r=>r.json()).then(d=>document.getElementById('builtin-led-status').innerHTML=d.message)}";
  script += "function ledFade(){fetch('/api/builtin-led-control?action=fade').then(r=>r.json()).then(d=>document.getElementById('builtin-led-status').innerHTML=d.message)}";
  script += "function ledOff(){fetch('/api/builtin-led-control?action=off').then(r=>r.json()).then(d=>document.getElementById('builtin-led-status').innerHTML=d.message)}";

  // NeoPixel
  script += "function configNeoPixel(){fetch('/api/neopixel-config?gpio='+document.getElementById('neoGPIO').value+'&count='+document.getElementById('neoCount').value)";
  script += ".then(r=>r.json()).then(d=>{document.getElementById('neopixel-status').innerHTML=d.message})}";
  script += "function testNeoPixel(){fetch('/api/neopixel-test').then(r=>r.json()).then(d=>document.getElementById('neopixel-status').innerHTML=d.result)}";
  script += "function neoPattern(p){fetch('/api/neopixel-pattern?pattern='+p).then(r=>r.json()).then(d=>document.getElementById('neopixel-status').innerHTML=d.message)}";
  script += "function neoCustomColor(){const c=document.getElementById('neoColor').value;";
  script += "const r=parseInt(c.substr(1,2),16),g=parseInt(c.substr(3,2),16),b=parseInt(c.substr(5,2),16);";
  script += "fetch('/api/neopixel-color?r='+r+'&g='+g+'&b='+b).then(r=>r.json()).then(d=>document.getElementById('neopixel-status').innerHTML=d.message)}";

  // OLED
  script += "function testOLED(){document.getElementById('oled-status').innerHTML='" + String(T().testing) + " (25s)...';";
  script += "fetch('/api/oled-test').then(r=>r.json()).then(d=>document.getElementById('oled-status').innerHTML=d.result)}";
  script += "function oledStep(step){document.getElementById('oled-status').innerHTML='" + String(T().testing) + "';";
  script += "fetch('/api/oled-step?step='+step).then(r=>r.json()).then(d=>{document.getElementById('oled-status').innerHTML=d.message;if(!d.success){document.getElementById('oled-status').innerHTML='❌ '+d.message}})}";
  script += "function oledMessage(){fetch('/api/oled-message?message='+encodeURIComponent(document.getElementById('oledMsg').value))";
  script += ".then(r=>r.json()).then(d=>document.getElementById('oled-status').innerHTML=d.message)}";
  script += "function configOLED(){const status=document.getElementById('oled-status');const pins=document.getElementById('oled-pins');";
  script += "const sda=document.getElementById('oledSDA').value;const scl=document.getElementById('oledSCL').value;";
  script += "status.innerHTML='" + String(T().reconfiguring) + "';";
  script += "fetch('/api/oled-config?sda='+sda+'&scl='+scl).then(r=>r.json()).then(d=>{";
  script += "if(d.success){status.innerHTML='✅ '+(d.message||'" + String(T().config_updated) + "');if(pins){pins.innerHTML='SDA:'+sda+' SCL:'+scl;}}";
  script += "else{status.innerHTML='❌ '+(d.message||'" + String(T().config_error) + "');}}).catch(e=>{status.innerHTML='❌ " + String(T().error_prefix) + "'+e;})";

  // Tests avancés
  script += "function testADC(){document.getElementById('adc-status').innerHTML='" + String(T().testing) + "';";
  script += "fetch('/api/adc-test').then(r=>r.json()).then(data=>{let h='';";
  script += "data.readings.forEach(a=>{h+='<div class=\'info-item\'><div class=\'info-label\'>GPIO '+a.pin+'</div><div class=\'info-value\'>'+a.raw+' ('+a.voltage.toFixed(2)+'V)</div></div>'});";
  script += "document.getElementById('adc-results').innerHTML=h;document.getElementById('adc-status').innerHTML=data.result})}";

  script += "function testTouch(){document.getElementById('touch-status').innerHTML='" + String(T().testing) + "';";
  script += "fetch('/api/touch-test').then(r=>r.json()).then(data=>{let h='';";
  script += "data.readings.forEach(t=>{h+='<div class=\'info-item\'><div class=\'info-label\'>Touch'+t.pin+'</div><div class=\'info-value\'>'+t.value+'</div></div>'});";
  script += "document.getElementById('touch-results').innerHTML=h;document.getElementById('touch-status').innerHTML=data.result})}";

  script += "function testPWM(){document.getElementById('pwm-status').innerHTML='" + String(T().testing) + "';";
  script += "fetch('/api/pwm-test').then(r=>r.json()).then(d=>document.getElementById('pwm-status').innerHTML=d.result)}";
  script += "function scanSPI(){document.getElementById('spi-status').innerHTML='" + String(T().scanning) + "';";
  script += "fetch('/api/spi-scan').then(r=>r.json()).then(d=>document.getElementById('spi-status').innerHTML=d.info)}";
  script += "function listPartitions(){document.getElementById('partitions-results').innerHTML='" + String(T().scanning) + "';";
  script += "fetch('/api/partitions-list').then(r=>r.json()).then(d=>document.getElementById('partitions-results').innerHTML=d.partitions)}";
  script += "function stressTest(){document.getElementById('stress-status').innerHTML='" + String(T().testing) + "';";
  script += "fetch('/api/stress-test').then(r=>r.json()).then(d=>document.getElementById('stress-status').innerHTML=d.result)}";

  // GPIO
  script += "function testAllGPIO(){document.getElementById('gpio-status').innerHTML='" + String(T().testing) + "';";
  script += "fetch('/api/test-gpio').then(r=>r.json()).then(data=>{let h='';";
  script += "data.results.forEach(g=>{h+='<div class=\'gpio-item '+(g.working?'gpio-ok':'gpio-fail')+'\'>GPIO '+g.pin+'<br>'+(g.working?'✅ " + String(T().ok) + "':'❌ " + String(T().fail) + "')+'</div>'});";
  script += "document.getElementById('gpio-results').innerHTML=h;document.getElementById('gpio-status').innerHTML='✅ " + String(T().completed) + " - '+data.results.length+' " + String(T().gpio) + " " + String(T().tested) + "'})}";

  script += "function updateText(id,value,fallback='—'){const el=document.getElementById(id);if(!el)return;if(value===undefined||value===null||value===''){el.textContent=fallback;}else{el.textContent=value;}}";
  script += "function refreshWirelessInfo(){fetch('/api/wireless-info').then(r=>r.json()).then(data=>updateWirelessInfo(data)).catch(e=>console.error('Wireless info',e));}";
  script += "function updateWirelessInfo(data){if(!data)return;const w=data.wifi||{};const bt=data.bluetooth||{};const connectedLabel=currentLang==='fr'?'Connecté':'Connected';const disconnectedLabel=currentLang==='fr'?'Déconnecté':'Disconnected';updateText('wifi-connection-state',w.connected?'✅ '+connectedLabel:'❌ '+disconnectedLabel,'—');updateText('wifi-ssid',w.ssid&&w.ssid.length?w.ssid:'—','—');const channelLabel=currentLang==='fr'?'Canal ':'Ch ';const channelValue=(typeof w.channel==='number'&&w.channel>0)?(channelLabel+w.channel):'—';updateText('wifi-channel',channelValue,'—');updateText('wifi-mode',w.mode&&w.mode.length?w.mode:'—','—');updateText('wifi-sleep',w.sleep&&w.sleep.length?w.sleep:'—','—');updateText('wifi-band',w.band&&w.band.length?w.band:'—','—');updateText('wifi-band-mode',w.band_mode&&w.band_mode.length?w.band_mode:'—','—');const txCode=(typeof w.tx_power_code==='number')?w.tx_power_code:'n/a';const txPowerText=(typeof w.tx_power_dbm==='number')?(w.tx_power_dbm.toFixed(1)+' dBm ('+txCode+')'):'—';updateText('wifi-tx-power',txPowerText,'—');updateText('wifi-hostname',w.hostname&&w.hostname.length?w.hostname:'—','—');if(w.connected){updateText('wifi-rssi',w.rssi+' dBm','—');updateText('wifi-quality',w.quality&&w.quality.length?w.quality:'—','—');updateText('wifi-ip',w.ip&&w.ip.length?w.ip:'—','—');updateText('wifi-subnet',w.subnet&&w.subnet.length?w.subnet:'—','—');updateText('wifi-gateway',w.gateway&&w.gateway.length?w.gateway:'—','—');updateText('wifi-dns',w.dns&&w.dns.length?w.dns:'—','—');}else{updateText('wifi-rssi','—','—');updateText('wifi-quality','—','—');updateText('wifi-ip','—','—');updateText('wifi-subnet','—','—');updateText('wifi-gateway','—','—');updateText('wifi-dns','—','—');}const caps=[];if(bt.classic)caps.push(currentLang==='fr'?'Classique':'Classic');if(bt.ble)caps.push('BLE');updateText('bluetooth-capabilities',caps.length?caps.join(', '):'—','—');updateText('bluetooth-controller',bt.controller&&bt.controller.length?bt.controller:'—','—');updateText('bluetooth-last-test',bt.last_test_message&&bt.last_test_message.length?bt.last_test_message:'—','—');const statusEl=document.getElementById('bluetooth-status');if(statusEl&&bt.last_test_message){statusEl.textContent=bt.last_test_message;}const hintEl=document.getElementById('bluetooth-hint');if(hintEl){if(bt.hint&&bt.hint.length){hintEl.textContent=bt.hint;}else{hintEl.textContent=' ';}}}";
  script += "function testBluetooth(){const status=document.getElementById('bluetooth-status');if(status){status.textContent='" + String(T().testing) + "';}fetch('/api/bluetooth-test').then(r=>r.json()).then(d=>{if(status){status.textContent=d.message||'" + String(T().unknown) + "';}refreshWirelessInfo();}).catch(e=>{if(status){status.textContent='❌ '+e;}});}";

  // WiFi
  script += "function scanWiFi(){document.getElementById('wifi-status').innerHTML='" + String(T().scanning) + "';";
  script += "fetch('/api/wifi-scan').then(r=>r.json()).then(data=>{let h='';";
  script += "data.networks.forEach(n=>{let s=n.rssi>=-60?'🟢':n.rssi>=-70?'🟡':'🔴';";
  script += "h+='<div class=\'wifi-item\'><div style=\'display:flex;justify-content:space-between\'><div><strong>'+s+' '+n.ssid+'</strong><br><small>'+n.bssid+' | Ch'+n.channel+' | '+n.encryption+'</small></div>';";
  script += "h+='<div style=\'font-size:1.2em;font-weight:bold\'>'+n.rssi+' dBm</div></div></div>'});";
  script += "document.getElementById('wifi-results').innerHTML=h;document.getElementById('wifi-status').innerHTML=data.networks.length+' " + String(T().networks) + "'})}";
  script += "refreshWirelessInfo();";

  // I2C
  script += "function scanI2C(){fetch('/api/i2c-scan').then(r=>r.json()).then(d=>{";
  script += "const count=document.getElementById('i2c-count');if(count){count.innerHTML=d.count+' " + String(T().devices) + "';}";
  script += "const devices=document.getElementById('i2c-devices');if(devices){devices.innerHTML=d.devices&&d.devices.length?d.devices:'—';}";
  script += "console.log('I2C: '+d.count+' périphérique(s)\\\n'+d.devices);}).catch(e=>console.error('I2C scan',e))}";

  // Benchmarks
  script += "function runBenchmarks(){";
  script += "document.getElementById('cpu-bench').innerHTML='" + String(T().testing) + "';";
  script += "document.getElementById('mem-bench').innerHTML='" + String(T().testing) + "';";
  script += "fetch('/api/benchmark').then(r=>r.json()).then(data=>{";
  script += "document.getElementById('cpu-bench').innerHTML=data.cpu+' us';";
  script += "document.getElementById('mem-bench').innerHTML=data.memory+' us';";
  script += "document.getElementById('cpu-perf').innerHTML=data.cpuPerf.toFixed(2)+' MFLOPS';";
  script += "document.getElementById('mem-speed').innerHTML=data.memSpeed.toFixed(2)+' MB/s'})}";

  script += "refreshWirelessInfo();";

  return script;
}

#endif  // APP_SCRIPT_H
