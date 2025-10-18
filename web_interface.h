/*
 * WEB_INTERFACE.H - Interface Web Dynamique v4.0.7
 */

#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

#include <WebServer.h>

// Déclarations externes (définies dans le fichier principal)
extern const char* DIAGNOSTIC_VERSION_STR;
extern const char* MDNS_HOSTNAME_STR;
extern WebServer server;
extern DiagnosticInfo diagnosticData;
extern Language currentLanguage;

// Déclarations forward des fonctions
String generateHTML();
String generateJavaScript();

// Implémentation de handleRoot()
void handleRoot() {
  server.send(200, "text/html; charset=utf-8", generateHTML());
}

// Implémentation de handleJavaScript()
void handleJavaScript() {
  server.send(200, "application/javascript; charset=utf-8", generateJavaScript());
}

// Génère le HTML principal
String generateHTML() {
  String html = "<!DOCTYPE html><html lang='fr'><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>ESP32 Diagnostic v";
  html += DIAGNOSTIC_VERSION_STR;
  html += "</title>";
  html += "<style>";
  html += "*{margin:0;padding:0;box-sizing:border-box}";
  html += "body{font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',sans-serif;";
  html += "background:linear-gradient(135deg,#667eea 0%,#764ba2 100%);min-height:100vh;padding:20px}";
  html += ".container{max-width:1400px;margin:0 auto;background:#fff;border-radius:20px;";
  html += "box-shadow:0 20px 60px rgba(0,0,0,.3);overflow:hidden}";
  html += ".header{background:linear-gradient(135deg,#667eea 0%,#764ba2 100%);";
  html += "color:#fff;padding:30px;text-align:center;position:relative}";
  html += ".header h1{font-size:2.5em;margin-bottom:10px;animation:fadeIn 0.5s}";
  html += "@keyframes fadeIn{from{opacity:0;transform:translateY(-20px)}to{opacity:1;transform:translateY(0)}}";
  html += ".lang-switcher{position:absolute;top:20px;right:20px;display:flex;gap:5px}";
  html += ".lang-btn{padding:8px 15px;background:rgba(255,255,255,.2);border:2px solid rgba(255,255,255,.3);";
  html += "border-radius:5px;color:#fff;cursor:pointer;font-weight:bold;transition:all .3s}";
  html += ".lang-btn:hover{background:rgba(255,255,255,.3)}";
  html += ".lang-btn.active{background:rgba(255,255,255,.4);border-color:rgba(255,255,255,.6)}";
  html += ".status-indicator{display:inline-block;width:12px;height:12px;border-radius:50%;";
  html += "margin-right:8px;animation:pulse 2s infinite}";
  html += ".status-online{background:#0f0;box-shadow:0 0 10px #0f0}";
  html += ".status-offline{background:#f00;box-shadow:0 0 10px #f00}";
  html += "@keyframes pulse{0%,100%{opacity:1}50%{opacity:0.5}}";
  html += ".nav{display:flex;justify-content:center;gap:10px;margin-top:20px;flex-wrap:wrap}";
  html += ".nav-btn{padding:10px 20px;background:rgba(255,255,255,.2);border:none;";
  html += "border-radius:5px;color:#fff;cursor:pointer;font-weight:bold;transition:all .3s}";
  html += ".nav-btn:hover{background:rgba(255,255,255,.3);transform:translateY(-2px)}";
  html += ".nav-btn.active{background:rgba(255,255,255,.4)}";
  html += ".content{padding:30px;animation:fadeIn 0.5s}";
  html += ".tab-content{display:none;animation:fadeIn 0.3s}";
  html += ".tab-content.active{display:block}";
  html += ".section{background:#f8f9fa;border-radius:15px;padding:25px;margin-bottom:20px;";
  html += "border-left:5px solid #667eea;transition:transform .3s}";
  html += ".section:hover{transform:translateX(5px)}";
  html += ".section h2{color:#667eea;margin-bottom:20px;font-size:1.5em;display:flex;";
  html += "align-items:center;gap:10px}";
  html += ".section h3{color:#667eea;margin:15px 0 10px;font-size:1.2em}";
  html += ".info-grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(300px,1fr));gap:15px}";
  html += ".info-item{background:#fff;padding:15px;border-radius:10px;border:1px solid #e0e0e0;";
  html += "transition:all .3s}";
  html += ".info-item:hover{transform:translateY(-2px);box-shadow:0 5px 15px rgba(0,0,0,.1)}";
  html += ".info-label{font-weight:bold;color:#667eea;margin-bottom:5px;font-size:.9em;";
  html += "text-transform:uppercase;letter-spacing:0.5px}";
  html += ".info-value{font-size:1.1em;color:#333;font-weight:500}";
  html += ".badge{display:inline-block;padding:5px 15px;border-radius:20px;font-size:.9em;";
  html += "font-weight:bold;animation:fadeIn 0.5s}";
  html += ".badge-success{background:#d4edda;color:#155724}";
  html += ".badge-warning{background:#fff3cd;color:#856404}";
  html += ".badge-danger{background:#f8d7da;color:#721c24}";
  html += ".badge-info{background:#d1ecf1;color:#0c5460}";
  html += ".btn{padding:12px 24px;border:none;border-radius:8px;font-size:1em;font-weight:bold;";
  html += "cursor:pointer;margin:5px;transition:all .3s;text-decoration:none;display:inline-block}";
  html += ".btn:hover{opacity:.9;transform:translateY(-2px);box-shadow:0 5px 15px rgba(0,0,0,.2)}";
  html += ".btn-primary{background:linear-gradient(135deg,#667eea 0%,#764ba2 100%);color:#fff}";
  html += ".btn-success{background:linear-gradient(135deg,#56ab2f 0%,#a8e063 100%);color:#fff}";
  html += ".btn-info{background:linear-gradient(135deg,#3a7bd5 0%,#00d2ff 100%);color:#fff}";
  html += ".btn-danger{background:linear-gradient(135deg,#eb3349 0%,#f45c43 100%);color:#fff}";
  html += ".btn-warning{background:linear-gradient(135deg,#f2994a 0%,#f2c94c 100%);color:#fff}";
  html += ".progress-bar{background:#e0e0e0;border-radius:10px;height:25px;overflow:hidden;";
  html += "margin-top:10px;position:relative}";
  html += ".progress-fill{height:100%;border-radius:10px;transition:width .5s ease;";
  html += "background:linear-gradient(90deg,#667eea 0%,#764ba2 100%);display:flex;";
  html += "align-items:center;justify-content:center;color:#fff;font-weight:bold;font-size:0.85em}";
  html += ".card{background:#fff;border-radius:10px;padding:20px;margin:10px 0;";
  html += "border:2px solid #e0e0e0;transition:all .3s}";
  html += ".card:hover{border-color:#667eea;box-shadow:0 5px 15px rgba(102,126,234,.2)}";
  html += ".loading{display:inline-block;width:20px;height:20px;border:3px solid #f3f3f3;";
  html += "border-top:3px solid #667eea;border-radius:50%;animation:spin 1s linear infinite}";
  html += "@keyframes spin{0%{transform:rotate(0deg)}100%{transform:rotate(360deg)}}";
  html += ".status-live{padding:15px;background:#f0f0f0;border-radius:10px;text-align:center;";
  html += "font-weight:bold;margin:15px 0;border-left:4px solid #667eea}";
  html += ".inline-feedback{margin-top:10px;font-size:0.9em;opacity:0;transition:opacity .3s;color:#0c5460;}";
  html += ".inline-feedback.show{opacity:1;}";
  html += ".inline-feedback.success{color:#155724;}";
  html += ".inline-feedback.error{color:#721c24;}";
  html += ".inline-feedback.info{color:#0c5460;}";
  html += ".gpio-grid{display:grid;grid-template-columns:repeat(auto-fill,minmax(80px,1fr));";
  html += "gap:10px;margin-top:15px}";
  html += ".gpio-item{padding:15px;background:#fff;border:2px solid #ddd;border-radius:8px;";
  html += "text-align:center;font-weight:bold;transition:all .3s}";
  html += ".gpio-item:hover{transform:scale(1.05)}";
  html += ".gpio-ok{border-color:#28a745;background:#d4edda}";
  html += ".gpio-fail{border-color:#dc3545;background:#f8d7da}";
  html += ".wifi-list{max-height:500px;overflow-y:auto}";
  html += ".wifi-item{background:#fff;padding:15px;margin:10px 0;border-radius:10px;";
  html += "border-left:4px solid #667eea;transition:all .3s}";
  html += ".wifi-item:hover{transform:translateX(5px);box-shadow:0 5px 15px rgba(0,0,0,.1)}";
  html += "input[type='number'],input[type='color'],input[type='text']{padding:10px;";
  html += "border:2px solid #ddd;border-radius:5px;font-size:1em;transition:border .3s}";
  html += "input:focus{outline:none;border-color:#667eea}";
  html += ".update-indicator{position:fixed;top:20px;right:20px;padding:10px 20px;";
  html += "background:#667eea;color:#fff;border-radius:5px;font-weight:bold;opacity:0;";
  html += "transition:opacity .3s;z-index:1000}";
  html += ".update-indicator.show{opacity:1}";
  html += "@media(max-width:768px){";
  html += ".header h1{font-size:1.8em}";
  html += ".info-grid{grid-template-columns:1fr}";
  html += ".nav{flex-direction:column}";
  html += ".nav-btn{width:100%}";
  html += ".lang-switcher{position:static;margin-top:10px;justify-content:center}";
  html += "}";
  html += "@media print{";
  html += ".nav,.btn,.lang-switcher{display:none}";
  html += ".container{box-shadow:none}";
  html += "}";
  html += "</style>";
  html += "</head><body>";
  html += "<div class='update-indicator' id='updateIndicator'>Mise à jour...</div>";
  html += "<div class='container'>";
  html += "<div class='header'>";
  html += "<div class='lang-switcher'>";
  html += "<button class='lang-btn active' onclick='changeLang(\"fr\")'>FR</button>";
  html += "<button class='lang-btn' onclick='changeLang(\"en\")'>EN</button>";
  html += "</div>";
  html += "<h1 id='main-title'>";
  html += "<span class='status-indicator status-online' id='statusIndicator'></span>";
  html += "Diagnostic ESP32 v";
  html += DIAGNOSTIC_VERSION_STR;
  html += "</h1>";
  html += "<div style='font-size:1.2em;margin:10px 0' id='chipModel'>";
  html += diagnosticData.chipModel;
  html += "</div>";
  html += "<div style='font-size:.9em;opacity:.9;margin:10px 0'>";
  html += "Accès: <a href='http://";
  html += MDNS_HOSTNAME_STR;
  html += ".local' style='color:#fff;text-decoration:underline'><strong>http://";
  html += MDNS_HOSTNAME_STR;
  html += ".local</strong></a> ou <strong id='ipAddress'>";
  html += diagnosticData.ipAddress;
  html += "</strong></div>";
  html += "<div class='nav'>";
  html += "<button class='nav-btn active' onclick='showTab(\"overview\",event)'>Vue d'ensemble</button>";
  html += "<button class='nav-btn' onclick='showTab(\"leds\",event)'>LEDs</button>";
  html += "<button class='nav-btn' onclick='showTab(\"screens\",event)'>Écrans</button>";
  html += "<button class='nav-btn' onclick='showTab(\"tests\",event)'>Tests</button>";
  html += "<button class='nav-btn' onclick='showTab(\"gpio\",event)'>GPIO</button>";
  html += "<button class='nav-btn' onclick='showTab(\"wifi\",event)'>WiFi</button>";
  html += "<button class='nav-btn' onclick='showTab(\"benchmark\",event)'>Performance</button>";
  html += "<button class='nav-btn' onclick='showTab(\"export\",event)'>Export</button>";
  html += "</div>";
  html += "</div>";
  html += "<div class='content'>";
  html += "<div id='tabContainer'></div>";
  html += "</div>";
  html += "</div>";
  html += "<script src='/js/app.js'></script>";
  html += "</body></html>";

  return html;
}

// Génère le JavaScript principal
String generateJavaScript() {
  String js = "console.log('ESP32 Diagnostic v";
  js += DIAGNOSTIC_VERSION_STR;
  js += " - Initialisation');";
  js += "const UPDATE_INTERVAL=5000;";
  js += "let currentLang='fr';";
  js += "let updateTimer=null;";
  js += "let isConnected=true;";
  js += "const inlineNoticeTimers={};";
  js += "let translations={};";

  // Initialisation - CORRIGÉE
  js += "document.addEventListener('DOMContentLoaded',function(){";
  js += "console.log('Interface chargée');";
  js += "loadAllData();";
  js += "startAutoUpdate();";
  js += "loadTab('overview');";
  js += "});";

  // Auto-update
  js += "function startAutoUpdate(){";
  js += "if(updateTimer)clearInterval(updateTimer);";
  js += "updateTimer=setInterval(()=>{if(isConnected)updateLiveData();},UPDATE_INTERVAL);";
  js += "}";

  // Load data
  js += "async function loadAllData(){";
  js += "showUpdateIndicator();";
  js += "try{";
  js += "await Promise.all([updateSystemInfo(),updateMemoryInfo(),updateWiFiInfo(),updatePeripheralsInfo()]);";
  js += "isConnected=true;updateStatusIndicator(true);";
  js += "}catch(error){console.error('Erreur:',error);isConnected=false;updateStatusIndicator(false);}";
  js += "hideUpdateIndicator();";
  js += "}";

  // Live data
  js += "async function updateLiveData(){";
  js += "try{";
  js += "const response=await fetch('/api/status');";
  js += "const data=await response.json();";
  js += "updateRealtimeValues(data);";
  js += "isConnected=true;updateStatusIndicator(true);";
  js += "}catch(error){console.error('Erreur:',error);isConnected=false;updateStatusIndicator(false);}";
  js += "}";

  // Update functions
  js += "async function updateSystemInfo(){";
  js += "const r=await fetch('/api/system-info');const d=await r.json();";
  js += "document.getElementById('chipModel').textContent=d.chipModel;";
  js += "document.getElementById('ipAddress').textContent=d.ipAddress;";
  js += "}";
  js += "async function updateMemoryInfo(){await fetch('/api/memory');}";
  js += "async function updateWiFiInfo(){await fetch('/api/wifi-info');}";
  js += "async function updatePeripheralsInfo(){await fetch('/api/peripherals');}";

  // Tab navigation - CORRIGÉ
  js += "function showTab(tabName,evt){";
  js += "document.querySelectorAll('.tab-content').forEach(t=>t.classList.remove('active'));";
  js += "document.querySelectorAll('.nav-btn').forEach(b=>b.classList.remove('active'));";
  js += "if(evt&&evt.target)evt.target.classList.add('active');";
  js += "const tab=document.getElementById(tabName);";
  js += "if(tab){tab.classList.add('active');}else{loadTab(tabName);}";
  js += "}";

  // Load tab
  js += "async function loadTab(tabName){";
  js += "const c=document.getElementById('tabContainer');";
  js += "let tab=document.getElementById(tabName);";
  js += "if(!tab){tab=document.createElement('div');tab.id=tabName;tab.className='tab-content';c.appendChild(tab);}";
  js += "tab.innerHTML='<div class=\"section\"><div class=\"loading\"></div><p style=\"text-align:center\">Chargement...</p></div>';";
  js += "tab.classList.add('active');";
  js += "try{";
  js += "if(tabName==='overview'){const r=await fetch('/api/overview');const d=await r.json();tab.innerHTML=buildOverview(d);}";
  js += "else if(tabName==='leds'){const r=await fetch('/api/leds-info');const d=await r.json();tab.innerHTML=buildLeds(d);}";
  js += "else if(tabName==='screens'){const r=await fetch('/api/screens-info');const d=await r.json();tab.innerHTML=buildScreens(d);}";
  js += "else if(tabName==='tests'){tab.innerHTML=buildTests();}";
  js += "else if(tabName==='gpio'){tab.innerHTML=buildGpio();}";
  js += "else if(tabName==='wifi'){tab.innerHTML=buildWifi();}";
  js += "else if(tabName==='benchmark'){tab.innerHTML=buildBenchmark();}";
  js += "else if(tabName==='export'){tab.innerHTML=buildExport();}";
  js += "}catch(e){tab.innerHTML='<div class=\"section\"><h2>❌ Erreur</h2><p>'+e+'</p></div>';}";
  js += "}";

  // Build Overview - COMPLET
  js += "function buildOverview(d){";
  js += "let h='<div class=\"section\"><h2>🔧 Informations Processeur</h2><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">Modèle complet</div><div class=\"info-value\">'+d.chip.model+' Rev'+d.chip.revision+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">CPU</div><div class=\"info-value\">'+d.chip.cores+' cœurs @ '+d.chip.freq+' MHz</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">MAC WiFi</div><div class=\"info-value\">'+d.chip.mac+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">Uptime</div><div class=\"info-value\" id=\"uptime\">'+formatUptime(d.chip.uptime)+'</div></div>';";
  js += "if(d.chip.temperature!==-999)h+='<div class=\"info-item\"><div class=\"info-label\">Température CPU</div><div class=\"info-value\" id=\"temperature\">'+d.chip.temperature.toFixed(1)+' °C</div></div>';";
  js += "h+='</div></div>';";

  js += "h+='<div class=\"section\"><h2>💾 Mémoire Détaillée</h2>';";
  js += "h+='<h3>Flash (Carte)</h3><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">Taille réelle</div><div class=\"info-value\">'+(d.memory.flash.real/1048576).toFixed(2)+' MB</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">Type Flash</div><div class=\"info-value\">'+d.memory.flash.type+'</div></div>';";
  js += "h+='</div>';";

  js += "h+='<h3>SRAM (Interne)</h3><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">Total</div><div class=\"info-value\" id=\"sram-total\">'+(d.memory.sram.total/1024).toFixed(2)+' KB</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">Libre</div><div class=\"info-value\" id=\"sram-free\">'+(d.memory.sram.free/1024).toFixed(2)+' KB</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">Utilisée</div><div class=\"info-value\" id=\"sram-used\">'+(d.memory.sram.used/1024).toFixed(2)+' KB</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">Fragmentation</div><div class=\"info-value\" id=\"fragmentation\">'+d.memory.fragmentation.toFixed(1)+'%</div></div>';";
  js += "h+='</div>';";
  js += "const sramPct=((d.memory.sram.used/d.memory.sram.total)*100).toFixed(1);";
  js += "h+='<div class=\"progress-bar\"><div class=\"progress-fill\" id=\"sram-progress\" style=\"width:'+sramPct+'%\">'+sramPct+'%</div></div>';";

  js += "if(d.memory.psram.total>0){";
  js += "h+='<h3>PSRAM (Externe)</h3><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">Total</div><div class=\"info-value\" id=\"psram-total\">'+(d.memory.psram.total/1048576).toFixed(2)+' MB</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">Libre</div><div class=\"info-value\" id=\"psram-free\">'+(d.memory.psram.free/1048576).toFixed(2)+' MB</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">Utilisée</div><div class=\"info-value\" id=\"psram-used\">'+(d.memory.psram.used/1048576).toFixed(2)+' MB</div></div>';";
  js += "h+='</div>';";
  js += "const psramPct=((d.memory.psram.used/d.memory.psram.total)*100).toFixed(1);";
  js += "h+='<div class=\"progress-bar\"><div class=\"progress-fill\" id=\"psram-progress\" style=\"width:'+psramPct+'%\">'+psramPct+'%</div></div>';";
  js += "}";
  js += "h+='</div>';";

  js += "h+='<div class=\"section\"><h2>📡 Connexion WiFi</h2><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">SSID</div><div class=\"info-value\">'+d.wifi.ssid+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">RSSI</div><div class=\"info-value\">'+d.wifi.rssi+' dBm</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">Qualité Signal</div><div class=\"info-value\">'+d.wifi.quality+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">Adresse IP</div><div class=\"info-value\">'+d.wifi.ip+'</div></div>';";
  js += "h+='</div></div>';";

  js += "h+='<div class=\"section\"><h2>🔌 GPIO et Interfaces</h2><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">Total GPIO</div><div class=\"info-value\">'+d.gpio.total+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">Périphériques I2C</div><div class=\"info-value\">'+d.gpio.i2c_count+'</div></div>';";
  js += "h+='<div class=\"info-item\" style=\"grid-column:1/-1\"><div class=\"info-label\">Adresses I2C</div><div class=\"info-value\">'+d.gpio.i2c_devices+'</div></div>';";
  js += "h+='</div></div>';";
  js += "return h;";
  js += "}";

  // Build LEDs - COMPLET
  js += "function buildLeds(d){";
  js += "const builtinPin=d.builtin.pin>=0?'GPIO '+d.builtin.pin:(translations.led_not_configured||'Non configurée');";
  js += "let h='<div class=\"section\"><h2>💡 LED Intégrée</h2><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">GPIO</div><div class=\"info-value\">'+builtinPin+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">Statut</div><div class=\"info-value\" id=\"builtin-led-status\">'+d.builtin.status+'</div></div>';";
  js += "if(d.neopixel&&d.neopixel.pin===d.builtin.pin&&d.builtin.pin>=0){h+='<div class=\"info-item\" style=\"grid-column:1/-1\"><div class=\"info-label\">'+(translations.warning||'Attention')+'</div><div class=\"info-value\">'+(translations.led_conflict||'Conflit NeoPixel (GPIO partagé)')+'</div></div>'; }";
  js += "h+='<div class=\"info-item\" style=\"grid-column:1/-1;text-align:center\">';";
  js += "h+='<div style=\"margin-bottom:10px\">GPIO <input type=\"number\" id=\"builtinLedPin\" value=\"'+(d.builtin.pin>=0?d.builtin.pin:0)+'\" min=\"0\" max=\"48\" style=\"width:80px\"> <button class=\"btn btn-info\" onclick=\"configBuiltinLED()\">⚙️ '+(translations.config||'Config')+'</button></div>';";
  js += "h+='<div><button class=\"btn btn-primary\" onclick=\"testBuiltinLED()\">🧪 '+(translations.test||'Test complet')+'</button> ';";
  js += "h+='<button class=\"btn btn-success\" onclick=\"ledBlink()\">⚡ '+(translations.blink||'Clignoter')+'</button> ';";
  js += "h+='<button class=\"btn btn-info\" onclick=\"ledFade()\">🌊 '+(translations.fade||'Fade')+'</button> ';";
  js += "h+='<button class=\"btn btn-warning\" onclick=\"ledOn()\">💡 '+(translations.led_on_state||'Allumer')+'</button> ';";
  js += "h+='<button class=\"btn btn-danger\" onclick=\"ledOff()\">⭕ '+(translations.off||'Éteindre')+'</button></div>';";
  js += "h+='<div class=\"inline-feedback\" id=\"builtin-led-feedback\"></div>';";
  js += "h+='</div></div></div>';";

  js += "const neoPin=d.neopixel.pin>=0?'GPIO '+d.neopixel.pin:(translations.led_not_configured||'Non configurée');";
  js += "h+='<div class=\"section\"><h2>🌈 NeoPixel / WS2812B</h2><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">GPIO</div><div class=\"info-value\">'+neoPin+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">Nombre de LEDs</div><div class=\"info-value\">'+d.neopixel.count+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">Statut</div><div class=\"info-value\" id=\"neopixel-status\">'+d.neopixel.status+'</div></div>';";
  js += "h+='<div class=\"info-item\" style=\"grid-column:1/-1;text-align:center\">';";
  js += "h+='<div style=\"margin-bottom:10px\">GPIO <input type=\"number\" id=\"neoGPIO\" value=\"'+(d.neopixel.pin>=0?d.neopixel.pin:0)+'\" min=\"0\" max=\"48\" style=\"width:80px\"> ';";
  js += "h+='<input type=\"number\" id=\"neoCount\" value=\"'+d.neopixel.count+'\" min=\"1\" max=\"100\" style=\"width:80px\"> ';";
  js += "h+='<button class=\"btn btn-info\" onclick=\"configNeoPixel()\">⚙️ '+(translations.config||'Config')+'</button></div>';";
  js += "h+='<div><button class=\"btn btn-primary\" onclick=\"testNeoPixel()\">🧪 '+(translations.test||'Test complet')+'</button></div>';";
  js += "h+='<div class=\"inline-feedback\" id=\"neopixel-feedback\"></div>';";
  js += "h+='<div style=\"margin-top:15px\"><strong>Animations :</strong><br>';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"neoPattern(\\'rainbow\\')\">🌈 Arc-en-ciel</button> ';";
  js += "h+='<button class=\"btn btn-success\" onclick=\"neoPattern(\\'blink\\')\">⚡ Clignoter</button> ';";
  js += "h+='<button class=\"btn btn-info\" onclick=\"neoPattern(\\'fade\\')\">🌊 Fade</button> ';";
  js += "h+='<button class=\"btn btn-warning\" onclick=\"neoPattern(\\'chase\\')\">🏃 Chase</button><br><br>';";
  js += "h+='<strong>Couleur personnalisée :</strong><br>';";
  js += "h+='<input type=\"color\" id=\"neoColor\" value=\"#ff0000\" style=\"height:50px;width:120px;border:none;border-radius:5px;cursor:pointer\"> ';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"neoCustomColor()\">🎨 Appliquer couleur</button><br><br>';";
  js += "h+='<button class=\"btn btn-danger\" onclick=\"neoPattern(\\'off\\')\">⭕ Tout éteindre</button>';";
  js += "h+='</div></div></div>';";
  js += "return h;";
  js += "}";

  js += "function buildScreens(d){";
  js += "const oled=(d&&d.oled)?d.oled:{};";
  js += "const pins=oled.pins||{};";
  js += "const status=oled.status||(translations.no_detected||'Aucun écran détecté');";
  js += "let h='<div class=\\\"section\\\"><h2>'+ (translations.oled_screen||'Écran OLED 0.96\\\\\" I2C')+'</h2><div class=\\\"info-grid\\\">';";
  js += "h+='<div class=\\\"info-item\\\"><div class=\\\"info-label\\\">'+(translations.status||'Statut')+'</div><div class=\\\"info-value\\\" id=\\\"oled-status\\\">'+status+'</div></div>';";
  js += "h+='<div class=\\\"info-item\\\"><div class=\\\"info-label\\\">'+(translations.i2c_pins||'Pins I2C')+'</div><div class=\\\"info-value\\\">SDA:'+(typeof pins.sda!==\"undefined\"?pins.sda:'?')+' SCL:'+(typeof pins.scl!==\"undefined\"?pins.scl:'?')+'</div></div>';";
  js += "h+='<div class=\"info-item\" style=\"grid-column:1/-1;text-align:center\">';";
  js += "h+='<div style=\"margin-bottom:10px\">SDA <input type=\"number\" id=\"oledSDA\" value=\"'+(typeof pins.sda!==\"undefined\"?pins.sda:21)+'\" min=\"0\" max=\"48\" style=\"width:70px\"> ';";
  js += "h+='SCL <input type=\"number\" id=\"oledSCL\" value=\"'+(typeof pins.scl!==\"undefined\"?pins.scl:22)+'\" min=\"0\" max=\"48\" style=\"width:70px\"> ';";
  js += "h+='<button class=\\\"btn btn-info\\\" onclick=\\\"configOLED()\\\">⚙️ '+(translations.apply_redetect||'Configurer')+'</button></div>';";
  js += "h+='<div class=\\\"inline-feedback\\\" id=\\\"oled-feedback\\\"></div>';";
  js += "h+='</div>';";
  js += "if(oled&&oled.available===false){";
  js += "h+='<div class=\\\"info-item\\\" style=\\\"grid-column:1/-1\\\"><div class=\\\"info-value\\\" style=\\\"background:#fff3cd;color:#856404;padding:12px;border-radius:10px;\\\">'+(translations.no_detected||'Aucun écran OLED détecté. Vérifiez le câblage et relancez la détection.')+'</div></div>';";
  js += "}";
  js += "h+='<div class=\\\"info-item\\\" style=\\\"grid-column:1/-1;text-align:center\\\">';";
  js += "h+='<div><button class=\\\"btn btn-primary\\\" onclick=\\\"testOLED()\\\">🧪 '+(translations.full_test||'Test complet')+' (25s)</button></div>';";
  js += "h+='<div style=\"margin-top:10px\"><input type=\"text\" id=\"oledText\" placeholder=\"'+(translations.custom_message||'Message à afficher')+'\" style=\"width:300px;padding:10px\"> ';";
  js += "h+='<button class=\"btn btn-success\" onclick=\"oledDisplayText()\">📤 '+(translations.show_message||'Afficher message')+'</button></div>';";
  js += "h+='<div style=\"margin-top:15px\">';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"oledPattern(\\'intro\\')\">🏁 '+(translations.oled_intro||'Accueil')+'</button> ';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"oledPattern(\\'large\\')\">🔠 '+(translations.oled_large_text||'Texte géant')+'</button> ';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"oledPattern(\\'fonts\\')\">🔤 '+(translations.oled_fonts||'Tailles')+'</button> ';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"oledPattern(\\'shapes\\')\">⬛ '+(translations.oled_shapes||'Formes')+'</button>';";
  js += "h+='</div>';";
  js += "h+='<div style=\"margin-top:8px\">';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"oledPattern(\\'lines\\')\">〰️ '+(translations.oled_lines||'Lignes')+'</button> ';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"oledPattern(\\'diagonals\\')\">⤴️ '+(translations.oled_diagonals||'Diagonales')+'</button> ';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"oledPattern(\\'square\\')\">🔲 '+(translations.oled_animation||'Carré')+'</button> ';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"oledPattern(\\'progress\\')\">📊 '+(translations.oled_progress||'Progression')+'</button>';";
  js += "h+='</div>';";
  js += "h+='<div style=\"margin-top:8px\">';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"oledPattern(\\'scroll\\')\">📜 '+(translations.oled_scroll||'Défilement')+'</button> ';";
  js += "h+='<button class=\"btn btn-danger\" onclick=\"oledPattern(\\'final\\')\">🏁 '+(translations.oled_final||'Final')+'</button>';";
  js += "h+='</div>';";
  js += "h+='</div></div></div>';";
  js += "return h;";
  js += "}";

  // Build Tests
  js += "function buildTests(){";
  js += "let h='<div class=\"section\"><h2>📊 Test ADC</h2>';";
  js += "h+='<div style=\"text-align:center;margin:20px 0\"><button class=\"btn btn-primary\" onclick=\"testADC()\">🧪 Lancer test ADC</button>';";
  js += "h+='<div id=\"adc-status\" class=\"status-live\">Cliquez pour tester</div></div>';";
  js += "h+='<div id=\"adc-results\" class=\"info-grid\"></div></div>';";
  js += "h+='<div class=\"section\"><h2>🔥 Stress Test</h2>';";
  js += "h+='<div style=\"text-align:center;margin:20px 0\">';";
  js += "h+='<p style=\"color:#dc3545;font-weight:bold\">⚠️ Peut ralentir l\\'ESP32 temporairement</p>';";
  js += "h+='<button class=\"btn btn-danger\" onclick=\"runStressTest()\">🚀 Démarrer Stress Test</button>';";
  js += "h+='<div id=\"stress-status\" class=\"status-live\">Non testé</div></div>';";
  js += "h+='<div id=\"stress-results\" class=\"info-grid\"></div></div>';";
  js += "return h;";
  js += "}";

  // Build GPIO
  js += "function buildGpio(){";
  js += "let h='<div class=\\\"section\\\"><h2>🔌 '+(translations.gpio_test||'Test GPIO')+'</h2>';";
  js += "h+='<div style=\\\"text-align:center;margin:20px 0\\\"><button class=\\\"btn btn-primary\\\" onclick=\\\"testAllGPIO()\\\">🧪 '+(translations.test_all_gpio||'Tester tous les GPIO')+'</button>';";
  js += "h+='<div id=\\\"gpio-status\\\" class=\\\"status-live\\\">'+(translations.click_to_test||'Cliquez pour tester')+'</div></div>';";
  js += "h+='<p id=\\\"gpio-warning\\\" style=\\\"margin:10px 0;padding:12px;border-radius:10px;background:#fff3cd;color:#856404;border-left:4px solid #f2c94c\\\">'+(translations.gpio_fail_hint||'Un GPIO \"FAIL\" peut être réservé. Vérifiez sa fonction avant de conclure qu\'il est défectueux.')+'</p>';";
  js += "h+='<div id=\\\"gpio-results\\\" class=\\\"gpio-grid\\\"></div></div>';";
  js += "return h;";
  js += "}";

  // Build WiFi
  js += "function buildWifi(){";
  js += "let h='<div class=\"section\"><h2>📡 Scanner WiFi</h2>';";
  js += "h+='<div style=\"text-align:center;margin:20px 0\"><button class=\"btn btn-primary\" onclick=\"scanWiFi()\">🔍 Scanner les réseaux</button>';";
  js += "h+='<div id=\"wifi-status\" class=\"status-live\">Cliquez pour scanner</div></div>';";
  js += "h+='<div id=\"wifi-results\" class=\"wifi-list\"></div></div>';";
  js += "return h;";
  js += "}";

  // Build Benchmark
  js += "function buildBenchmark(){";
  js += "let h='<div class=\"section\"><h2>⚡ Benchmarks</h2>';";
  js += "h+='<div style=\"text-align:center;margin:20px 0\"><button class=\"btn btn-primary\" onclick=\"runBenchmarks()\">🚀 Lancer les benchmarks</button></div>';";
  js += "h+='<div id=\"benchmark-results\" class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">CPU</div><div class=\"info-value\" id=\"cpu-bench\">Non testé</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">Mémoire</div><div class=\"info-value\" id=\"mem-bench\">Non testé</div></div>';";
  js += "h+='</div></div>';";
  js += "return h;";
  js += "}";

  // Build Export
  js += "function buildExport(){";
  js += "let h='<div class=\"section\"><h2>💾 Exporter les données</h2>';";
  js += "h+='<div style=\"display:grid;grid-template-columns:repeat(auto-fit,minmax(250px,1fr));gap:20px;margin-top:20px\">';";
  js += "h+='<div class=\"card\" style=\"text-align:center;padding:30px\"><h3 style=\"color:#667eea\">📄 TXT</h3>';";
  js += "h+='<p style=\"font-size:0.9em;color:#666;margin:15px 0\">Rapport lisible</p>';";
  js += "h+='<a href=\"/export/txt\" class=\"btn btn-primary\">📥 Télécharger TXT</a></div>';";
  js += "h+='<div class=\"card\" style=\"text-align:center;padding:30px\"><h3 style=\"color:#3a7bd5\">📋 JSON</h3>';";
  js += "h+='<p style=\"font-size:0.9em;color:#666;margin:15px 0\">Format structuré</p>';";
  js += "h+='<a href=\"/export/json\" class=\"btn btn-info\">📥 Télécharger JSON</a></div>';";
  js += "h+='<div class=\"card\" style=\"text-align:center;padding:30px\"><h3 style=\"color:#56ab2f\">📊 CSV</h3>';";
  js += "h+='<p style=\"font-size:0.9em;color:#666;margin:15px 0\">Pour Excel</p>';";
  js += "h+='<a href=\"/export/csv\" class=\"btn btn-success\">📥 Télécharger CSV</a></div>';";
  js += "h+='<div class=\"card\" style=\"text-align:center;padding:30px\"><h3 style=\"color:#667eea\">🖨️ Print</h3>';";
  js += "h+='<p style=\"font-size:0.9em;color:#666;margin:15px 0\">Version PDF</p>';";
  js += "h+='<a href=\"/print\" target=\"_blank\" class=\"btn btn-primary\">🖨️ Ouvrir</a></div>';";
  js += "h+='</div></div>';";
  js += "return h;";
  js += "}";

  // FONCTIONS API - LED
  js += "async function configBuiltinLED(){";
  js += "const input=document.getElementById('builtinLedPin');";
  js += "if(!input)return;";
  js += "try{";
  js += "const r=await fetch('/api/builtin-led-config?gpio='+input.value);";
  js += "const d=await r.json();";
  js += "if(d.message){document.getElementById('builtin-led-status').textContent=d.message;showInlineNotice('builtin-led-feedback',d.message,d.success?'success':'error');}}";
  js += "catch(e){showInlineNotice('builtin-led-feedback','Erreur configuration','error');}";
  js += "}";
  js += "async function testBuiltinLED(){";
  js += "document.getElementById('builtin-led-status').textContent='Test en cours...';";
  js += "const r=await fetch('/api/builtin-led-test');const d=await r.json();";
  js += "document.getElementById('builtin-led-status').textContent=d.result;showInlineNotice('builtin-led-feedback',d.result,d.success?'success':'error');";
  js += "}";
  js += "async function ledBlink(){";
  js += "const r=await fetch('/api/builtin-led-control?action=blink');const d=await r.json();";
  js += "document.getElementById('builtin-led-status').textContent=d.message;";
  js += "}";
  js += "async function ledFade(){";
  js += "const r=await fetch('/api/builtin-led-control?action=fade');const d=await r.json();";
  js += "document.getElementById('builtin-led-status').textContent=d.message;";
  js += "}";
  js += "async function ledOff(){";
  js += "const r=await fetch('/api/builtin-led-control?action=off');const d=await r.json();";
  js += "document.getElementById('builtin-led-status').textContent=d.message;";
  js += "}";
  js += "async function ledOn(){";
  js += "const r=await fetch('/api/builtin-led-control?action=on');";
  js += "const d=await r.json();";
  js += "document.getElementById('builtin-led-status').textContent=d.message;";
  js += "}";

  // NeoPixel configuration
  js += "async function configNeoPixel(){";
  js += "const gpio=document.getElementById('neoGPIO');";
  js += "const count=document.getElementById('neoCount');";
  js += "if(!gpio||!count)return;";
  js += "try{";
  js += "const r=await fetch('/api/neopixel-config?gpio='+gpio.value+'&count='+count.value);";
  js += "const d=await r.json();";
  js += "if(d.message){document.getElementById('neopixel-status').textContent=d.message;showInlineNotice('neopixel-feedback',d.message,d.success?'success':'error');}}";
  js += "catch(e){showInlineNotice('neopixel-feedback','Erreur configuration NeoPixel','error');}";
  js += "}";

  // NeoPixel couleur personnalisée
  js += "async function neoCustomColor(){";
  js += "const color=document.getElementById('neoColor').value;";
  js += "const r=parseInt(color.substr(1,2),16);";
  js += "const g=parseInt(color.substr(3,2),16);";
  js += "const b=parseInt(color.substr(5,2),16);";
  js += "document.getElementById('neopixel-status').textContent='Application couleur RGB('+r+','+g+','+b+')...';";
  js += "const resp=await fetch('/api/neopixel-color?r='+r+'&g='+g+'&b='+b);";
  js += "const d=await resp.json();";
  js += "document.getElementById('neopixel-status').textContent=d.message;showInlineNotice('neopixel-feedback',d.message,d.success?'success':'info');";
  js += "}";
  // FONCTIONS API - NeoPixel
  js += "async function testNeoPixel(){";
  js += "document.getElementById('neopixel-status').textContent='Test en cours...';";
  js += "const r=await fetch('/api/neopixel-test');const d=await r.json();";
  js += "document.getElementById('neopixel-status').textContent=d.result;showInlineNotice('neopixel-feedback',d.result,d.success?'success':'info');";
  js += "}";
  js += "async function neoPattern(p){";
  js += "const r=await fetch('/api/neopixel-pattern?pattern='+p);const d=await r.json();";
  js += "document.getElementById('neopixel-status').textContent=d.message;showInlineNotice('neopixel-feedback',d.message,d.success?'success':'error');";
  js += "}";

  // FONCTIONS API - OLED
  js += "async function configOLED(){";
  js += "const sda=document.getElementById('oledSDA');";
  js += "const scl=document.getElementById('oledSCL');";
  js += "if(!sda||!scl)return;";
  js += "try{";
  js += "const r=await fetch('/api/oled-config?sda='+sda.value+'&scl='+scl.value);";
  js += "const d=await r.json();";
  js += "if(d.message){document.getElementById('oled-status').textContent=d.message;showInlineNotice('oled-feedback',d.message,d.success?'success':'error');}}";
  js += "catch(e){showInlineNotice('oled-feedback','Erreur configuration OLED','error');}";
  js += "}";
  js += "async function testOLED(){";
  js += "document.getElementById('oled-status').textContent='Test en cours (25s)...';";
  js += "const r=await fetch('/api/oled-test');const d=await r.json();";
  js += "document.getElementById('oled-status').textContent=d.result;showInlineNotice('oled-feedback',d.result,d.success?'success':'error');";
  js += "}";
  js += "async function oledPattern(p){";
  js += "const status=document.getElementById('oled-status');";
  js += "if(status)status.textContent='Lancement motif...';";
  js += "const r=await fetch('/api/oled-pattern?pattern='+encodeURIComponent(p));";
  js += "const d=await r.json();";
  js += "if(status)status.textContent=d.message||status.textContent;";
  js += "if(d.message)showInlineNotice('oled-feedback',d.message,d.success?'success':'error');";
  js += "}";

  js += "async function oledDisplayText(){";
  js += "const text=document.getElementById('oledText');";
  js += "if(!text)return;";
  js += "if(!text.value){showInlineNotice('oled-feedback',translations.message_required||'Entrez un message !','error');return;}";
  js += "document.getElementById('oled-status').textContent='Affichage en cours...';";
  js += "const r=await fetch('/api/oled-message?message='+encodeURIComponent(text.value));";
  js += "const d=await r.json();";
  js += "document.getElementById('oled-status').textContent=d.message;showInlineNotice('oled-feedback',d.message,d.success?'success':'error');";
  js += "}";

  // FONCTIONS API - Tests
  js += "async function testADC(){";
  js += "document.getElementById('adc-status').textContent='Test en cours...';";
  js += "const r=await fetch('/api/adc-test');const d=await r.json();";
  js += "let h='';d.readings.forEach(rd=>{h+='<div class=\"info-item\"><div class=\"info-label\">GPIO '+rd.pin+'</div><div class=\"info-value\">'+rd.raw+' ('+rd.voltage.toFixed(2)+'V)</div></div>';});";
  js += "document.getElementById('adc-results').innerHTML=h;";
  js += "document.getElementById('adc-status').textContent=d.result;";
  js += "}";

  js += "async function testAllGPIO(){";
  js += "document.getElementById('gpio-status').textContent='Test en cours...';";
  js += "const r=await fetch('/api/test-gpio');const d=await r.json();";
  js += "let h='';d.results.forEach(g=>{h+='<div class=\"gpio-item '+(g.working?'gpio-ok':'gpio-fail')+'\">GPIO '+g.pin+'<br>'+(g.working?'✅ OK':'❌ FAIL')+'</div>';});";
  js += "document.getElementById('gpio-results').innerHTML=h;";
  js += "document.getElementById('gpio-status').textContent='Terminé - '+d.results.length+' GPIO testés';";
  js += "}";

  js += "async function scanWiFi(){";
  js += "document.getElementById('wifi-status').textContent='Scan en cours...';";
  js += "const r=await fetch('/api/wifi-scan');const d=await r.json();";
  js += "let h='';d.networks.forEach(n=>{";
  js += "const icon=n.rssi>=-60?'🟢':n.rssi>=-70?'🟡':'🔴';";
  js += "const color=n.rssi>=-60?'#28a745':n.rssi>=-70?'#ffc107':'#dc3545';";
  js += "h+='<div class=\"wifi-item\"><div style=\"display:flex;justify-content:space-between\"><div><strong>'+icon+' '+n.ssid+'</strong><br><small>'+n.bssid+' | Canal '+n.channel+'</small></div>';";
  js += "h+='<div style=\"font-size:1.3em;font-weight:bold;color:'+color+'\">'+n.rssi+' dBm</div></div></div>';";
  js += "});";
  js += "document.getElementById('wifi-results').innerHTML=h;";
  js += "document.getElementById('wifi-status').textContent=d.networks.length+' réseaux détectés';";
  js += "}";

  js += "async function runBenchmarks(){";
  js += "document.getElementById('cpu-bench').textContent='Test en cours...';";
  js += "document.getElementById('mem-bench').textContent='Test en cours...';";
  js += "const r=await fetch('/api/benchmark');const d=await r.json();";
  js += "document.getElementById('cpu-bench').textContent=d.cpu+' µs';";
  js += "document.getElementById('mem-bench').textContent=d.memory+' µs';";
  js += "}";

  js += "async function runStressTest(){";
  js += "document.getElementById('stress-status').textContent='⚠️ Test en cours... Patientez';";
  js += "document.getElementById('stress-results').innerHTML='<div class=\"loading\"></div>';";
  js += "try{";
  js += "const r=await fetch('/api/stress-test');";
  js += "const d=await r.json();";
  js += "let h='';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">Temps CPU</div><div class=\"info-value\">'+d.cpu_time+' ms</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">Temps Mémoire</div><div class=\"info-value\">'+d.mem_time+' ms</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">Allocations</div><div class=\"info-value\">'+d.allocs_success+'/'+d.allocs_total+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">Fragmentation</div><div class=\"info-value\">'+d.fragmentation.toFixed(1)+'%</div></div>';";
  js += "document.getElementById('stress-results').innerHTML=h;";
  js += "document.getElementById('stress-status').textContent='✅ '+d.message;";
  js += "}catch(e){";
  js += "document.getElementById('stress-status').textContent='❌ Erreur: '+e;";
  js += "document.getElementById('stress-results').innerHTML='';";
  js += "}";
  js += "}";

  // Utility functions
  js += "function formatUptime(ms){";
  js += "const s=Math.floor(ms/1000),m=Math.floor(s/60),h=Math.floor(m/60),d=Math.floor(h/24);";
  js += "return d+'j '+(h%24)+'h '+(m%60)+'m';";
  js += "}";

  js += "function showUpdateIndicator(msg){const el=document.getElementById('updateIndicator');if(!el)return;if(!el.dataset.defaultText)el.dataset.defaultText=el.textContent;if(msg)el.textContent=msg;el.classList.add('show');}";
  js += "function hideUpdateIndicator(delay=500){const el=document.getElementById('updateIndicator');if(!el)return;setTimeout(()=>{el.classList.remove('show');if(el.dataset.defaultText)el.textContent=el.dataset.defaultText;},delay);}";
  js += "function showInlineNotice(id,message,type){const el=document.getElementById(id);if(!el||!message)return;const tone=type||'info';el.classList.remove('success','error','info');el.classList.add('inline-feedback',tone);el.classList.add('show');clearTimeout(inlineNoticeTimers[id]);inlineNoticeTimers[id]=setTimeout(()=>{el.classList.remove('show');},4000);}";
  js += "function updateStatusIndicator(c){";
  js += "const i=document.getElementById('statusIndicator');";
  js += "if(c){i.classList.remove('status-offline');i.classList.add('status-online');}";
  js += "else{i.classList.remove('status-online');i.classList.add('status-offline');}";
  js += "}";

  js += "function updateRealtimeValues(d){";
  js += "const u=document.getElementById('uptime');if(u)u.textContent=formatUptime(d.uptime);";
  js += "const t=document.getElementById('temperature');if(t&&d.temperature!==-999)t.textContent=d.temperature.toFixed(1)+' °C';";
  js += "const sf=document.getElementById('sram-free');if(sf)sf.textContent=(d.sram.free/1024).toFixed(2)+' KB';";
  js += "const su=document.getElementById('sram-used');if(su)su.textContent=(d.sram.used/1024).toFixed(2)+' KB';";
  js += "const sp=document.getElementById('sram-progress');";
  js += "if(sp&&d.sram.total>0){const pct=((d.sram.used/d.sram.total)*100).toFixed(1);sp.style.width=pct+'%';sp.textContent=pct+'%';}";
  js += "if(d.psram&&d.psram.total>0){";
  js += "const pf=document.getElementById('psram-free');if(pf)pf.textContent=(d.psram.free/1048576).toFixed(2)+' MB';";
  js += "const pu=document.getElementById('psram-used');if(pu)pu.textContent=(d.psram.used/1048576).toFixed(2)+' MB';";
  js += "const pp=document.getElementById('psram-progress');";
  js += "if(pp){const pct=((d.psram.used/d.psram.total)*100).toFixed(1);pp.style.width=pct+'%';pp.textContent=pct+'%';}";
  js += "}";
  js += "const f=document.getElementById('fragmentation');if(f)f.textContent=d.fragmentation.toFixed(1)+'%';";
  js += "}";

  // Changement de langue
  js += "function changeLang(lang){";
  js += "fetch('/api/set-language?lang='+lang).then(r=>r.json()).then(d=>{";
  js += "if(!d.success)throw new Error('lang-update');";
  js += "currentLang=lang;";
  js += "document.querySelectorAll('.lang-btn').forEach(b=>b.classList.remove('active'));";
  js += "document.querySelectorAll('.lang-btn').forEach(b=>{if(b.textContent.toLowerCase()===lang)b.classList.add('active');});";
  js += "return fetch('/api/get-translations');";
  js += "}).then(r=>r.json()).then(tr=>{";
  js += "translations=tr||{};";
  js += "updateInterfaceTexts(translations);";
  js += "const msg=(translations.language_changed||'Langue changée')+' : '+lang.toUpperCase();";
  js += "showUpdateIndicator(msg);hideUpdateIndicator(2000);";
  js += "}).catch(()=>{showUpdateIndicator('Erreur changement langue');hideUpdateIndicator(2500);});";
  js += "}";

  js += "function updateInterfaceTexts(t){";
  js += "const tabs=['overview','leds','screens','tests','gpio','wifi','benchmark','export'];";
  js += "const btns=document.querySelectorAll('.nav-btn');";
  js += "btns.forEach((btn,i)=>{if(t[tabs[i]])btn.textContent=t[tabs[i]];});";
  js += "const gpioWarn=document.getElementById('gpio-warning');if(gpioWarn&&t.gpio_fail_hint)gpioWarn.textContent=t.gpio_fail_hint;";
  js += "const oledInput=document.getElementById('oledText');if(oledInput&&t.custom_message)oledInput.placeholder=t.custom_message;";
  js += "}";

  return js;
}

#endif // WEB_INTERFACE_H
