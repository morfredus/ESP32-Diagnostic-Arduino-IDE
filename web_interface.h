/*
 * WEB_INTERFACE.H - Interface Web Dynamique v3.0-dev
 * Interface moderne avec mise à jour temps réel
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
  html += "<div class='update-indicator' id='updateIndicator' data-i18n='update_indicator' data-i18n-fallback='Mise à jour...'>Mise à jour...</div>";
  html += "<div class='container'>";
  html += "<div class='header'>";
  html += "<div class='lang-switcher'>";
  html += "<button class='lang-btn active' data-lang='fr' onclick='changeLang(\"fr\")'>FR</button>";
  html += "<button class='lang-btn' data-lang='en' onclick='changeLang(\"en\")'>EN</button>";
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
  html += "<button class='nav-btn active' data-i18n='nav_overview' data-i18n-fallback='Vue d&#39;ensemble' onclick='showTab(\"overview\",event)'>Vue d'ensemble</button>";
  html += "<button class='nav-btn' data-i18n='nav_leds' data-i18n-fallback='LEDs' onclick='showTab(\"leds\",event)'>LEDs</button>";
  html += "<button class='nav-btn' data-i18n='nav_screens' data-i18n-fallback='Écrans' onclick='showTab(\"screens\",event)'>Écrans</button>";
  html += "<button class='nav-btn' data-i18n='nav_sensors' data-i18n-fallback='Capteurs' onclick='showTab(\"sensors\",event)'>Capteurs</button>";
  html += "<button class='nav-btn' data-i18n='nav_tests' data-i18n-fallback='Tests' onclick='showTab(\"tests\",event)'>Tests</button>";
  html += "<button class='nav-btn' data-i18n='nav_gpio' data-i18n-fallback='GPIO' onclick='showTab(\"gpio\",event)'>GPIO</button>";
  html += "<button class='nav-btn' data-i18n='nav_wifi' data-i18n-fallback='WiFi' onclick='showTab(\"wifi\",event)'>WiFi</button>";
  html += "<button class='nav-btn' data-i18n='nav_benchmark' data-i18n-fallback='Performance' onclick='showTab(\"benchmark\",event)'>Performance</button>";
  html += "<button class='nav-btn' data-i18n='nav_export' data-i18n-fallback='Export' onclick='showTab(\"export\",event)'>Export</button>";
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
  js += "let translations={};";
  js += "let activeTab='overview';";
  js += "let updateTimer=null;";
  js += "let isConnected=true;";
  js += "let sensorsCache=null;";
  js += "const fallbackTranslations={fr:{loading:'Chargement...',error_title:'❌ Erreur',update_indicator:'Mise à jour...',language_changed:'Langue changée',language_error:'Erreur changement langue',click_to_scan:'Cliquez pour scanner',displaying:'Affichage en cours...',command_sent:'Commande envoyée',communication_error:'Erreur de communication',nav_sensors:'Capteurs',sensors_section:'Capteurs',pir_sensor:'Capteur PIR',distance_sensor:'Capteur distance',dht_sensor:'Capteur DHT',ldr_sensor:'Capteur LDR',trig_pin_label:'Broche TRIG',echo_pin_label:'Broche ECHO',sensor_type:'Type',temperature_label:'Température',humidity_label:'Humidité',voltage_label:'Tension',motion_detected:'Mouvement détecté',motion_none:'Pas de mouvement',distance_value:'Distance',light_level:'Luminosité',sensor_config:'Configuration capteur',apply_changes:'Appliquer',run_sensor_test:'Tester le capteur'},en:{loading:'Loading...',error_title:'❌ Error',update_indicator:'Updating...',language_changed:'Language changed',language_error:'Language change error',click_to_scan:'Click to scan',displaying:'Displaying...',command_sent:'Command sent',communication_error:'Communication error',nav_sensors:'Sensors',sensors_section:'Sensors',pir_sensor:'PIR sensor',distance_sensor:'Distance sensor',dht_sensor:'DHT sensor',ldr_sensor:'LDR sensor',trig_pin_label:'TRIG pin',echo_pin_label:'ECHO pin',sensor_type:'Type',temperature_label:'Temperature',humidity_label:'Humidity',voltage_label:'Voltage',motion_detected:'Motion detected',motion_none:'No motion',distance_value:'Distance',light_level:'Light level',sensor_config:'Sensor configuration',apply_changes:'Apply',run_sensor_test:'Run sensor test'}};";
  js += "function tr(key,fallback){const langFallback=fallbackTranslations[currentLang]||{};return (translations[key]||langFallback[key]||fallback||key);}";

  // Initialisation - CORRIGÉE
  js += "document.addEventListener('DOMContentLoaded',async function(){";
  js += "console.log('Interface chargée');";
  js += "await initTranslations();";
  js += "updateInterfaceTexts();";
  js += "loadAllData();";
  js += "startAutoUpdate();";
  js += "await loadTab('overview');";
  js += "});";

  // Gestion des traductions
  js += "async function fetchTranslations(lang=currentLang){";
  js += "const targetLang=lang||currentLang;";
  js += "try{";
  js += "const response=await fetch('/api/get-translations?lang='+encodeURIComponent(targetLang)+'&_='+Date.now(),{cache:'no-store'});";
  js += "if(!response.ok)throw new Error('HTTP '+response.status);";
  js += "return await response.json();";
  js += "}catch(error){console.error('Erreur chargement traductions',error);return fallbackTranslations[targetLang]||{};}";
  js += "}";
  js += "async function initTranslations(){translations=await fetchTranslations(currentLang);}";
  js += "function applyLanguageButtonState(){document.querySelectorAll('.lang-btn').forEach(btn=>{btn.classList.toggle('active',btn.dataset.lang===currentLang);});}";

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
  js += "if(tab){tab.classList.add('active');if(tab.dataset.lang!==currentLang){loadTab(tabName,true);}}else{loadTab(tabName);}";
  js += "}";

  // Load tab
  js += "async function loadTab(tabName,force=false){";
  js += "activeTab=tabName;";
  js += "const c=document.getElementById('tabContainer');";
  js += "let tab=document.getElementById(tabName);";
  js += "if(!tab){tab=document.createElement('div');tab.id=tabName;tab.className='tab-content';c.appendChild(tab);}";
  js += "tab.innerHTML='<div class=\"section\"><div class=\"loading\"></div><p style=\"text-align:center\">'+tr('loading','Chargement...')+'</p></div>';";
  js += "tab.classList.add('active');";
  js += "tab.dataset.lang=currentLang;";
  js += "try{";
  js += "if(tabName==='overview'){const r=await fetch('/api/overview');const d=await r.json();tab.innerHTML=buildOverview(d);}";
  js += "else if(tabName==='leds'){const r=await fetch('/api/leds-info');const d=await r.json();tab.innerHTML=buildLeds(d);}";
  js += "else if(tabName==='screens'){const r=await fetch('/api/screens-info');const d=await r.json();tab.innerHTML=buildScreens(d);}";
  js += "else if(tabName==='sensors'){const r=await fetch('/api/sensors-info');const d=await r.json();sensorsCache=d.sensors||{};tab.innerHTML=buildSensors(sensorsCache);}";
  js += "else if(tabName==='tests'){tab.innerHTML=buildTests();}";
  js += "else if(tabName==='gpio'){tab.innerHTML=buildGpio();}";
  js += "else if(tabName==='wifi'){tab.innerHTML=buildWifi();}";
  js += "else if(tabName==='benchmark'){tab.innerHTML=buildBenchmark();}";
  js += "else if(tabName==='export'){tab.innerHTML=buildExport();}";
  js += "}catch(e){tab.innerHTML='<div class=\"section\"><h2>'+tr('error_title','❌ Erreur')+'</h2><p>'+tr('error_generic','Une erreur est survenue')+'<br>'+e+'</p></div>';}";
  js += "}";

  // Build Overview - COMPLET
  js += "function buildOverview(d){";
  js += "let h='<div class=\"section\"><h2>🔧 '+tr('chip_info','Informations Chip Détaillées')+'</h2><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('full_model','Modèle complet')+'</div><div class=\"info-value\">'+d.chip.model+' Rev'+d.chip.revision+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('cpu_cores','CPU Cores')+'</div><div class=\"info-value\">'+d.chip.cores+' '+tr('cores','coeurs')+' @ '+d.chip.freq+' MHz</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('mac_wifi','MAC WiFi')+'</div><div class=\"info-value\">'+d.chip.mac+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('uptime','Uptime')+'</div><div class=\"info-value\" id=\"uptime\">'+formatUptime(d.chip.uptime)+'</div></div>';";
  js += "if(d.chip.temperature!==-999)h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('cpu_temp','Température CPU')+'</div><div class=\"info-value\" id=\"temperature\">'+d.chip.temperature.toFixed(1)+' °C</div></div>';";
  js += "h+='</div></div>';";
  js += "h+='<div class=\"section\"><h2>💾 '+tr('memory_details','Mémoire Détaillée')+'</h2>';";
  js += "h+='<h3>'+tr('flash_memory','Flash Memory')+'</h3><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('real_size','Taille réelle (carte)')+'</div><div class=\"info-value\">'+(d.memory.flash.real/1048576).toFixed(2)+' MB</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('flash_type','Type Flash')+'</div><div class=\"info-value\">'+d.memory.flash.type+'</div></div>';";
  js += "h+='</div>';";
  js += "h+='<h3>'+tr('internal_sram','SRAM Interne')+'</h3><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('total_size','Total')+'</div><div class=\"info-value\" id=\"sram-total\">'+(d.memory.sram.total/1024).toFixed(2)+' KB</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('free','Libre')+'</div><div class=\"info-value\" id=\"sram-free\">'+(d.memory.sram.free/1024).toFixed(2)+' KB</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('used','Utilisée')+'</div><div class=\"info-value\" id=\"sram-used\">'+(d.memory.sram.used/1024).toFixed(2)+' KB</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('memory_fragmentation','Fragmentation Mémoire')+'</div><div class=\"info-value\" id=\"fragmentation\">'+d.memory.fragmentation.toFixed(1)+'%</div></div>';";
  js += "h+='</div>';";
  js += "const sramPct=((d.memory.sram.used/d.memory.sram.total)*100).toFixed(1);";
  js += "h+='<div class=\"progress-bar\"><div class=\"progress-fill\" id=\"sram-progress\" style=\"width:'+sramPct+'%\">'+sramPct+'%</div></div>';";
  js += "if(d.memory.psram.total>0){";
  js += "h+='<h3>'+tr('psram_external','PSRAM (Mémoire Externe)')+'</h3><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('total_size','Total')+'</div><div class=\"info-value\" id=\"psram-total\">'+(d.memory.psram.total/1048576).toFixed(2)+' MB</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('free','Libre')+'</div><div class=\"info-value\" id=\"psram-free\">'+(d.memory.psram.free/1048576).toFixed(2)+' MB</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('used','Utilisée')+'</div><div class=\"info-value\" id=\"psram-used\">'+(d.memory.psram.used/1048576).toFixed(2)+' MB</div></div>';";
  js += "h+='</div>';";
  js += "const psramPct=((d.memory.psram.used/d.memory.psram.total)*100).toFixed(1);";
  js += "h+='<div class=\"progress-bar\"><div class=\"progress-fill\" id=\"psram-progress\" style=\"width:'+psramPct+'%\">'+psramPct+'%</div></div>';";
  js += "}";
  js += "h+='</div>';";
  js += "h+='<div class=\"section\"><h2>📡 '+tr('wifi_connection','Connexion WiFi Détaillée')+'</h2><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('connected_ssid','SSID Connecté')+'</div><div class=\"info-value\">'+d.wifi.ssid+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('signal_power','Puissance Signal (RSSI)')+'</div><div class=\"info-value\">'+d.wifi.rssi+' dBm</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('signal_quality','Qualité Signal')+'</div><div class=\"info-value\">'+d.wifi.quality+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('ip_address','Adresse IP')+'</div><div class=\"info-value\">'+d.wifi.ip+'</div></div>';";
  js += "h+='</div></div>';";
  js += "h+='<div class=\"section\"><h2>🔌 '+tr('gpio_interfaces','GPIO et Interfaces')+'</h2><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('total_gpio','Total GPIO')+'</div><div class=\"info-value\">'+d.gpio.total+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('i2c_peripherals','Périphériques I2C')+'</div><div class=\"info-value\">'+d.gpio.i2c_count+'</div></div>';";
  js += "h+='<div class=\"info-item\" style=\"grid-column:1/-1\"><div class=\"info-label\">'+tr('detected_addresses','Adresses Détectées')+'</div><div class=\"info-value\">'+d.gpio.i2c_devices+'</div></div>';";
  js += "h+='</div></div>';";
  js += "if(d.sensors){const s=d.sensors;h+='<div class=\\"section\\"><h2>🧭 '+tr('sensors_section','Capteurs')+'</h2><div class=\\"info-grid\\">';";
  js += "h+='<div class=\\"info-item\\"><div class=\\"info-label\\">'+tr('pir_sensor','Capteur PIR')+'</div><div class=\\"info-value\\">'+formatSensorStatus(s.pir,'pir')+'</div></div>';";
  js += "h+='<div class=\\"info-item\\"><div class=\\"info-label\\">'+tr('distance_sensor','Capteur distance')+'</div><div class=\\"info-value\\">'+formatSensorStatus(s.distance,'distance')+'</div></div>';";
  js += "h+='<div class=\\"info-item\\"><div class=\\"info-label\\">'+tr('dht_sensor','Capteur DHT')+'</div><div class=\\"info-value\\">'+formatSensorStatus(s.dht,'dht')+'</div></div>';";
  js += "h+='<div class=\\"info-item\\"><div class=\\"info-label\\">'+tr('ldr_sensor','Capteur LDR')+'</div><div class=\\"info-value\\">'+formatSensorStatus(s.ldr,'ldr')+'</div></div>';";
  js += "h+='</div></div>'; }";
  js += "return h;";
  js += "}";

  // Build LEDs - COMPLET
  js += "function buildLeds(d){";
  js += "let h='<div class=\"section\"><h2>💡 '+tr('builtin_led','LED Intégrée')+'</h2><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('gpio','GPIO')+'</div><div class=\"info-value\">GPIO '+d.builtin.pin+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('status','Statut')+'</div><div class=\"info-value\" id=\"builtin-led-status\">'+d.builtin.status+'</div></div>';";
  js += "h+='<div class=\"info-item\" style=\"grid-column:1/-1;text-align:center\">';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"testBuiltinLED()\">🧪 '+tr('full_test','Test complet')+'</button> ';";
  js += "h+='<button class=\"btn btn-success\" onclick=\"ledBlink()\">⚡ '+tr('blink','Blink')+'</button> ';";
  js += "h+='<button class=\"btn btn-info\" onclick=\"ledFade()\">🌊 '+tr('fade','Fade')+'</button> ';";
  js += "h+='<button class=\"btn btn-warning\" onclick=\"ledOn()\">💡 '+tr('enabled','Activée')+'</button> ';";
  js += "h+='<button class=\"btn btn-danger\" onclick=\"ledOff()\">⭕ '+tr('off','Off')+'</button>';";
  js += "h+='</div></div></div>';";
  js += "h+='<div class=\"section\"><h2>🌈 '+tr('neopixel','NeoPixel / WS2812B')+'</h2><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('gpio','GPIO')+'</div><div class=\"info-value\">GPIO '+d.neopixel.pin+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('device_count','Nombre de périphériques')+'</div><div class=\"info-value\">'+d.neopixel.count+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('status','Statut')+'</div><div class=\"info-value\" id=\"neopixel-status\">'+d.neopixel.status+'</div></div>';";
  js += "h+='<div class=\"info-item\" style=\"grid-column:1/-1;text-align:center\">';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"testNeoPixel()\">🧪 '+tr('full_test','Test complet')+'</button><br><br>';";
  js += "h+='<strong>'+tr('animations','Animations :')+'</strong><br>';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"neoPattern(\\'rainbow\\')\">🌈 '+tr('rainbow','Arc-en-ciel')+'</button> ';";
  js += "h+='<button class=\"btn btn-success\" onclick=\"neoPattern(\\'blink\\')\">⚡ '+tr('blink','Blink')+'</button> ';";
  js += "h+='<button class=\"btn btn-info\" onclick=\"neoPattern(\\'fade\\')\">🌊 '+tr('fade','Fade')+'</button> ';";
  js += "h+='<button class=\"btn btn-warning\" onclick=\"neoPattern(\\'chase\\')\">🏃 Chase</button><br><br>';";
  js += "h+='<strong>'+tr('custom_color','Couleur personnalisée :')+'</strong><br>';";
  js += "h+='<input type=\"color\" id=\"neoColor\" value=\"#ff0000\" style=\"height:50px;width:120px;border:none;border-radius:5px;cursor:pointer\"> ';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"neoCustomColor()\">🎨 '+tr('apply_color','Appliquer couleur')+'</button><br><br>';";
  js += "h+='<button class=\"btn btn-danger\" onclick=\"neoPattern(\\'off\\')\">⭕ '+tr('turn_off_all','Tout éteindre')+'</button>';";
  js += "h+='</div></div></div>';";
  js += "return h;";
  js += "}";

js += "function buildScreens(d){";
  js += "let h='<div class=\"section\"><h2>📺 '+tr('tft_screen','Écran TFT 320x240')+'</h2><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('status','Statut')+'</div><div class=\"info-value\" id=\"tft-status\">'+d.tft.status+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('resolution','Résolution')+'</div><div class=\"info-value\">'+d.tft.width+'x'+d.tft.height+'</div></div>';";
  js += "h+='<div class=\"info-item\" style=\"grid-column:1/-1;text-align:center\">';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"testTFT()\">🧪 '+tr('full_test','Test complet')+' (15s)</button> ';";
  js += "h+='<button class=\"btn btn-success\" onclick=\"tftPattern(\\'colors\\')\">🎨 '+tr('colors','Couleurs')+'</button> ';";
  js += "h+='<button class=\"btn btn-info\" onclick=\"tftPattern(\\'checkerboard\\')\">⬛ '+tr('checkerboard','Damier')+'</button> ';";
  js += "h+='<button class=\"btn btn-danger\" onclick=\"tftPattern(\\'clear\\')\">🗑️ '+tr('clear','Effacer')+'</button>';";
  js += "h+='<br><br><input type=\"text\" id=\"tftText\" placeholder=\"'+tr('custom_message','Message personnalisé')+'\" style=\"width:300px;padding:10px\">';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"tftDisplayText()\">📤 '+tr('display_text','Afficher texte')+'</button>';";
  js += "h+='</div></div></div>';";
  js += "h+='<div class=\"section\"><h2>🖥️ '+tr('oled_screen','Écran OLED 0.96\" I2C')+'</h2><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('status','Statut')+'</div><div class=\"info-value\" id=\"oled-status\">'+d.oled.status+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('i2c_pins','Pins I2C')+'</div><div class=\"info-value\">SDA:'+d.oled.pins.sda+' SCL:'+d.oled.pins.scl+'</div></div>';";
  js += "h+='<div class=\"info-item\" style=\"grid-column:1/-1\"><div class=\"info-label\">'+tr('sensor_config','Configuration capteur')+'</div><div class=\"info-value\">';";
  js += "h+=tr('trig_pin_label','Broche TRIG').replace('TRIG','SDA')+': <input type=\"number\" id=\"oled-sda\" min=\"0\" max=\"48\" value=\"'+d.oled.pins.sda+'\" style=\"width:80px;margin-right:10px\">';";
  js += "h+=tr('echo_pin_label','Broche ECHO').replace('ECHO','SCL')+': <input type=\"number\" id=\"oled-scl\" min=\"0\" max=\"48\" value=\"'+d.oled.pins.scl+'\" style=\"width:80px\">';";
  js += "h+='</div></div>';";
  js += "h+='<div class=\"info-item\" style=\"grid-column:1/-1;text-align:center\">';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"testOLED()\">🧪 '+tr('full_test','Test complet')+' (25s)</button> ';";
  js += "h+='<button class=\"btn btn-success\" onclick=\"oledPattern(\\'info\\')\">ℹ️ '+tr('info_label','Infos')+'</button> ';";
  js += "h+='<button class=\"btn btn-info\" onclick=\"oledPattern(\\'shapes\\')\">🔷 '+tr('shapes_label','Formes')+'</button> ';";
  js += "h+='<button class=\"btn btn-warning\" onclick=\"oledPattern(\\'grid\\')\"># '+tr('grid_label','Grille')+'</button> ';";
  js += "h+='<button class=\"btn btn-secondary\" onclick=\"oledPattern(\\'scroll\\')\">➡️ '+tr('scroll_label','Défilement')+'</button> ';";
  js += "h+='<button class=\"btn btn-danger\" onclick=\"oledPattern(\\'clear\\')\">🗑️ '+tr('clear','Effacer')+'</button>';";
  js += "h+='<br><button class=\"btn btn-info\" style=\"margin-top:10px\" onclick=\"applyOledPins()\">⚙️ '+tr('apply_changes','Appliquer')+'</button>';";
  js += "h+='<br><br><input type=\"text\" id=\"oledText\" placeholder=\"'+tr('custom_message','Message personnalisé')+'\" style=\"width:300px;padding:10px\">';";
  js += "h+='<button class=\"btn btn-success\" onclick=\"oledDisplayText()\">📤 '+tr('display_message','Afficher message')+'</button>';";
  js += "h+='</div></div></div>';";
  js += "return h;";
  js += "}";

  js += "function parseSensorNumber(value){const num=Number(value);return isNaN(num)?0:num;}";
  js += "function formatSensorStatus(sensor,key){if(!sensor)return tr('not_tested','Non testé');if(!sensor.tested)return tr('not_tested','Non testé');if(sensor.success){if(key==='pir'){return sensor.motion?tr('motion_detected','Mouvement détecté'):tr('motion_none','Pas de mouvement');}if(key==='distance'){return tr('distance_value','Distance')+': '+parseSensorNumber(sensor.distance_cm).toFixed(1)+' cm';}if(key==='dht'){return tr('temperature_label','Température')+': '+parseSensorNumber(sensor.temperature).toFixed(1)+'°C | '+tr('humidity_label','Humidité')+': '+parseSensorNumber(sensor.humidity).toFixed(1)+'%';}if(key==='ldr'){return tr('voltage_label','Tension')+': '+parseSensorNumber(sensor.voltage).toFixed(2)+' V';}}return sensor.status||tr('fail','FAIL');}";

  js += "function buildSensorSection(key,sensor,icon,title){sensor=sensor||{};let h='<div class=\\"section\\" id=\\"sensor-'+key+'\\">';h+='<h2>'+icon+' '+title+'</h2>';h+='<div class=\\"info-grid\\">';h+='<div class=\\"info-item\\"><div class=\\"info-label\\">'+tr('status','Statut')+'</div><div class=\\"info-value\\" id=\\"sensor-'+key+'-status\\">'+formatSensorStatus(sensor,key)+'</div></div>';if(key==='pir'){h+='<div class=\\"info-item\\"><div class=\\"info-label\\">'+tr('motion_detected','Mouvement détecté')+'</div><div class=\\"info-value\\" id=\\"sensor-pir-motion\\">'+(sensor.tested?(sensor.motion?tr('motion_detected','Mouvement détecté'):tr('motion_none','Pas de mouvement')):tr('not_tested','Non testé'))+'</div></div>'; }else if(key==='distance'){h+='<div class=\\"info-item\\"><div class=\\"info-label\\">'+tr('distance_value','Distance')+'</div><div class=\\"info-value\\" id=\\"sensor-distance-value\\">'+(sensor.success?parseSensorNumber(sensor.distance_cm).toFixed(1)+' cm':tr('not_tested','Non testé'))+'</div></div>'; }else if(key==='dht'){h+='<div class=\\"info-item\\"><div class=\\"info-label\\">'+tr('temperature_label','Température')+'</div><div class=\\"info-value\\" id=\\"sensor-dht-temp\\">'+(sensor.success?parseSensorNumber(sensor.temperature).toFixed(1)+'°C':tr('not_tested','Non testé'))+'</div></div>';h+='<div class=\\"info-item\\"><div class=\\"info-label\\">'+tr('humidity_label','Humidité')+'</div><div class=\\"info-value\\" id=\\"sensor-dht-humidity\\">'+(sensor.success?parseSensorNumber(sensor.humidity).toFixed(1)+'%':tr('not_tested','Non testé'))+'</div></div>'; }else if(key==='ldr'){h+='<div class=\\"info-item\\"><div class=\\"info-label\\">'+tr('light_level','Luminosité')+'</div><div class=\\"info-value\\" id=\\"sensor-ldr-voltage\\">'+(sensor.success?parseSensorNumber(sensor.voltage).toFixed(2)+' V':tr('not_tested','Non testé'))+'</div></div>';h+='<div class=\\"info-item\\"><div class=\\"info-label\\">RAW</div><div class=\\"info-value\\" id=\\"sensor-ldr-raw\\">'+(sensor.success?(sensor.raw||0):'-')+'</div></div>'; }
  js += "h+='</div>';h+='<div class=\\"info-grid\\">';if(key==='pir'){h+='<div class=\\"info-item\\"><div class=\\"info-label\\">'+tr('gpio','GPIO')+'</div><div class=\\"info-value\\"><input type=\\"number\\" id=\\"sensor-pir-pin\\" min=\\"0\\" max=\\"48\\" value=\\"'+(('pin'in sensor)?sensor.pin:'')+'\\" style=\\"width:100px\\"></div></div>'; }else if(key==='distance'){h+='<div class=\\"info-item\\"><div class=\\"info-label\\">'+tr('trig_pin_label','Broche TRIG')+'</div><div class=\\"info-value\\"><input type=\\"number\\" id=\\"sensor-distance-trig\\" min=\\"0\\" max=\\"48\\" value=\\"'+(('trig'in sensor)?sensor.trig:'')+'\\" style=\\"width:100px\\"></div></div>';h+='<div class=\\"info-item\\"><div class=\\"info-label\\">'+tr('echo_pin_label','Broche ECHO')+'</div><div class=\\"info-value\\"><input type=\\"number\\" id=\\"sensor-distance-echo\\" min=\\"0\\" max=\\"48\\" value=\\"'+(('echo'in sensor)?sensor.echo:'')+'\\" style=\\"width:100px\\"></div></div>'; }else if(key==='dht'){h+='<div class=\\"info-item\\"><div class=\\"info-label\\">'+tr('gpio','GPIO')+'</div><div class=\\"info-value\\"><input type=\\"number\\" id=\\"sensor-dht-pin\\" min=\\"0\\" max=\\"48\\" value=\\"'+(('pin'in sensor)?sensor.pin:'')+'\\" style=\\"width:100px;margin-right:10px\\"></div></div>';h+='<div class=\\"info-item\\"><div class=\\"info-label\\">'+tr('sensor_type','Type')+'</div><div class=\\"info-value\\"><select id=\\"sensor-dht-type\\" style=\\"padding:8px;border:2px solid #ddd;border-radius:5px\\">';
  js += "['DHT11','DHT22','DHT21'].forEach(t=>{h+='<option value=\\"'+t+'\\"'+(t===((sensor.type)||'DHT11')?' selected':'')+'>'+t+'</option>';});";
  js += "h+='</select></div></div>'; }else if(key==='ldr'){h+='<div class=\\"info-item\\"><div class=\\"info-label\\">'+tr('gpio','GPIO')+'</div><div class=\\"info-value\\"><input type=\\"number\\" id=\\"sensor-ldr-pin\\" min=\\"0\\" max=\\"48\\" value=\\"'+(('pin'in sensor)?sensor.pin:'')+'\\" style=\\"width:100px\\"></div></div>'; }
  js += "h+='</div>';h+='<div style=\\"text-align:center;margin-top:10px\\">';h+='<button class=\\"btn btn-info\\" onclick=\\"applySensorConfig(\\''+key+'\\')\">⚙️ '+tr('apply_changes','Appliquer')+'</button> ';h+='<button class=\\"btn btn-success\\" onclick=\\"runSensorTest(\\''+key+'\\')\">🧪 '+tr('run_sensor_test','Tester le capteur')+'</button>';h+='</div></div>';return h;}";

  js += "function buildSensors(data){sensorsCache=data||{};const pir=data?.pir||{};const distance=data?.distance||{};const dht=data?.dht||{};const ldr=data?.ldr||{};let h='';h+=buildSensorSection('pir',pir,'🕵️',tr('pir_sensor','Capteur PIR'));h+=buildSensorSection('distance',distance,'📏',tr('distance_sensor','Capteur distance'));h+=buildSensorSection('dht',dht,'🌡️',tr('dht_sensor','Capteur DHT'));h+=buildSensorSection('ldr',ldr,'🔦',tr('ldr_sensor','Capteur LDR'));return h;}";

  js += "function updateSensorUI(data){sensorsCache=data||{};const pir=data?.pir||{};const distance=data?.distance||{};const dht=data?.dht||{};const ldr=data?.ldr||{};const pirStatus=document.getElementById('sensor-pir-status');if(pirStatus)pirStatus.textContent=formatSensorStatus(pir,'pir');const pirMotion=document.getElementById('sensor-pir-motion');if(pirMotion)pirMotion.textContent=pir.tested?(pir.motion?tr('motion_detected','Mouvement détecté'):tr('motion_none','Pas de mouvement')):tr('not_tested','Non testé');const pirPin=document.getElementById('sensor-pir-pin');if(pirPin&&'pin'in pir)pirPin.value=pir.pin;const distStatus=document.getElementById('sensor-distance-status');if(distStatus)distStatus.textContent=formatSensorStatus(distance,'distance');const distValue=document.getElementById('sensor-distance-value');if(distValue)distValue.textContent=distance.success?parseSensorNumber(distance.distance_cm).toFixed(1)+' cm':(distance.tested?distance.status:tr('not_tested','Non testé'));const trigInput=document.getElementById('sensor-distance-trig');if(trigInput&&'trig'in distance)trigInput.value=distance.trig;const echoInput=document.getElementById('sensor-distance-echo');if(echoInput&&'echo'in distance)echoInput.value=distance.echo;const dhtStatus=document.getElementById('sensor-dht-status');if(dhtStatus)dhtStatus.textContent=formatSensorStatus(dht,'dht');const dhtTemp=document.getElementById('sensor-dht-temp');if(dhtTemp)dhtTemp.textContent=dht.success?parseSensorNumber(dht.temperature).toFixed(1)+'°C':(dht.tested?dht.status:tr('not_tested','Non testé'));const dhtHum=document.getElementById('sensor-dht-humidity');if(dhtHum)dhtHum.textContent=dht.success?parseSensorNumber(dht.humidity).toFixed(1)+'%':(dht.tested?dht.status:tr('not_tested','Non testé'));const dhtPin=document.getElementById('sensor-dht-pin');if(dhtPin&&'pin'in dht)dhtPin.value=dht.pin;const dhtType=document.getElementById('sensor-dht-type');if(dhtType&&'type'in dht)dhtType.value=dht.type;const ldrStatus=document.getElementById('sensor-ldr-status');if(ldrStatus)ldrStatus.textContent=formatSensorStatus(ldr,'ldr');const ldrVolt=document.getElementById('sensor-ldr-voltage');if(ldrVolt)ldrVolt.textContent=ldr.success?parseSensorNumber(ldr.voltage).toFixed(2)+' V':(ldr.tested?ldr.status:tr('not_tested','Non testé'));const ldrRaw=document.getElementById('sensor-ldr-raw');if(ldrRaw)ldrRaw.textContent=ldr.success?(ldr.raw||0):'-';const ldrPin=document.getElementById('sensor-ldr-pin');if(ldrPin&&'pin'in ldr)ldrPin.value=ldr.pin;}";

  js += "function setSensorStatus(key,message){const el=document.getElementById('sensor-'+key+'-status');if(el)el.textContent=message;}";

  js += "async function applySensorConfig(sensor){setSensorStatus(sensor,tr('update_indicator','Mise à jour...'));try{let url='/api/sensor-config?sensor='+sensor;if(sensor==='pir'){const pin=document.getElementById('sensor-pir-pin').value;url+='&pin='+pin;}else if(sensor==='distance'){const trig=document.getElementById('sensor-distance-trig').value;const echo=document.getElementById('sensor-distance-echo').value;url+='&trig='+trig+'&echo='+echo;}else if(sensor==='dht'){const pin=document.getElementById('sensor-dht-pin').value;const type=document.getElementById('sensor-dht-type').value;url+='&pin='+pin+'&type='+type;}else if(sensor==='ldr'){const pin=document.getElementById('sensor-ldr-pin').value;url+='&pin='+pin;}const r=await fetch(url);const d=await r.json();if(d.success&&d.sensors){updateSensorUI(d.sensors);setSensorStatus(sensor,formatSensorStatus(d.sensors[sensor],sensor));}else{setSensorStatus(sensor,d.message||tr('communication_error','Erreur de communication'));}}catch(e){setSensorStatus(sensor,tr('communication_error','Erreur de communication'));}}";

  js += "async function runSensorTest(sensor){setSensorStatus(sensor,tr('testing','Test en cours...'));try{const r=await fetch('/api/sensor-test?sensor='+sensor);const d=await r.json();if(d.success&&d.sensors){updateSensorUI(d.sensors);setSensorStatus(sensor,formatSensorStatus(d.sensors[sensor],sensor));}else{setSensorStatus(sensor,d.message||tr('communication_error','Erreur de communication'));}}catch(e){setSensorStatus(sensor,tr('communication_error','Erreur de communication'));}}";

  // Build Tests
  js += "function buildTests(){";
  js += "let h='<div class=\"section\"><h2>📊 '+tr('adc_test','Test ADC')+'</h2>';";
  js += "h+='<div style=\"text-align:center;margin:20px 0\"><button class=\"btn btn-primary\" onclick=\"testADC()\">🧪 '+tr('full_test','Test complet')+'</button>';";
  js += "h+='<div id=\"adc-status\" class=\"status-live\">'+tr('click_to_test','Cliquez pour tester')+'</div></div>';";
  js += "h+='<div id=\"adc-results\" class=\"info-grid\"></div></div>';";
  js += "h+='<div class=\"section\"><h2>🔥 '+tr('memory_stress','Stress Test Mémoire')+'</h2>';";
  js += "h+='<div style=\"text-align:center;margin:20px 0\">';";
  js += "h+='<p style=\"color:#dc3545;font-weight:bold\">⚠️ '+tr('stress_warning','Peut ralentir l\\'ESP32 temporairement')+'</p>';";
  js += "h+='<button class=\"btn btn-danger\" onclick=\"runStressTest()\">🚀 '+tr('start_stress','Démarrer Stress Test')+'</button>';";
  js += "h+='<div id=\"stress-status\" class=\"status-live\">'+tr('not_tested','Non testé')+'</div></div>';";
  js += "h+='<div id=\"stress-results\" class=\"info-grid\"></div></div>';";
  js += "return h;";
  js += "}";

  // Build GPIO
  js += "function buildGpio(){";
  js += "let h='<div class=\"section\"><h2>🔌 '+tr('gpio_test','Test GPIO')+'</h2>';";
  js += "h+='<div style=\"text-align:center;margin:20px 0\"><button class=\"btn btn-primary\" onclick=\"testAllGPIO()\">🧪 '+tr('test_all_gpio','Tester Tous les GPIO')+'</button>';";
  js += "h+='<div id=\"gpio-status\" class=\"status-live\">'+tr('click_to_test','Cliquez pour tester')+'</div></div>';";
  js += "h+='<div id=\"gpio-results\" class=\"gpio-grid\"></div></div>';";
  js += "return h;";
  js += "}";

  // Build WiFi
  js += "function buildWifi(){";
  js += "let h='<div class=\"section\"><h2>📡 '+tr('wifi_scanner','Scanner WiFi')+'</h2>';";
  js += "h+='<div style=\"text-align:center;margin:20px 0\"><button class=\"btn btn-primary\" onclick=\"scanWiFi()\">🔍 '+tr('scan_networks','Scanner Réseaux WiFi')+'</button>';";
  js += "h+='<div id=\"wifi-status\" class=\"status-live\">'+tr('click_to_scan','Cliquez pour scanner')+'</div></div>';";
  js += "h+='<div id=\"wifi-results\" class=\"wifi-list\"></div></div>';";
  js += "return h;";
  js += "}";

  // Build Benchmark
  js += "function buildBenchmark(){";
  js += "let h='<div class=\"section\"><h2>⚡ '+tr('performance_bench','Benchmarks de Performance')+'</h2>';";
  js += "h+='<div style=\"text-align:center;margin:20px 0\"><button class=\"btn btn-primary\" onclick=\"runBenchmarks()\">🚀 '+tr('run_benchmarks','Lancer Benchmarks')+'</button></div>';";
  js += "h+='<div id=\"benchmark-results\" class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('cpu_benchmark','CPU Benchmark')+'</div><div class=\"info-value\" id=\"cpu-bench\">'+tr('not_tested','Non testé')+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+tr('memory_benchmark','Mémoire Benchmark')+'</div><div class=\"info-value\" id=\"mem-bench\">'+tr('not_tested','Non testé')+'</div></div>';";
  js += "h+='</div></div>';";
  js += "return h;";
  js += "}";

  // Build Export
  js += "function buildExport(){";
  js += "let h='<div class=\"section\"><h2>💾 '+tr('data_export','Export des Données')+'</h2>';";
  js += "h+='<div style=\"display:grid;grid-template-columns:repeat(auto-fit,minmax(250px,1fr));gap:20px;margin-top:20px\">';";
  js += "h+='<div class=\"card\" style=\"text-align:center;padding:30px\"><h3 style=\"color:#667eea\">📄 '+tr('txt_file','Fichier TXT')+'</h3>';";
  js += "h+='<p style=\"font-size:0.9em;color:#666;margin:15px 0\">'+tr('readable_report','Rapport texte lisible')+'</p>';";
  js += "h+='<a href=\"/export/txt\" class=\"btn btn-primary\">📥 '+tr('download_txt','Télécharger TXT')+'</a></div>';";
  js += "h+='<div class=\"card\" style=\"text-align:center;padding:30px\"><h3 style=\"color:#3a7bd5\">📋 '+tr('json_file','Fichier JSON')+'</h3>';";
  js += "h+='<p style=\"font-size:0.9em;color:#666;margin:15px 0\">'+tr('structured_format','Format structuré')+'</p>';";
  js += "h+='<a href=\"/export/json\" class=\"btn btn-info\">📥 '+tr('download_json','Télécharger JSON')+'</a></div>';";
  js += "h+='<div class=\"card\" style=\"text-align:center;padding:30px\"><h3 style=\"color:#56ab2f\">📊 '+tr('csv_file','Fichier CSV')+'</h3>';";
  js += "h+='<p style=\"font-size:0.9em;color:#666;margin:15px 0\">'+tr('for_excel','Pour Excel')+'</p>';";
  js += "h+='<a href=\"/export/csv\" class=\"btn btn-success\">📥 '+tr('download_csv','Télécharger CSV')+'</a></div>';";
  js += "h+='<div class=\"card\" style=\"text-align:center;padding:30px\"><h3 style=\"color:#667eea\">🖨️ '+tr('printable_version','Version Imprimable')+'</h3>';";
  js += "h+='<p style=\"font-size:0.9em;color:#666;margin:15px 0\">'+tr('pdf_format','Format PDF')+'</p>';";
  js += "h+='<a href=\"/print\" target=\"_blank\" class=\"btn btn-primary\">🖨️ '+tr('open','Ouvrir')+'</a></div>';";
  js += "h+='</div></div>';";
  js += "return h;";
  js += "}";

  // FONCTIONS API - LED
  js += "async function testBuiltinLED(){";
  js += "document.getElementById('builtin-led-status').textContent=tr('testing','Test en cours...');";
  js += "const r=await fetch('/api/builtin-led-test');const d=await r.json();";
  js += "document.getElementById('builtin-led-status').textContent=d.result;alert(d.result);";
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

  // NeoPixel couleur personnalisée
  js += "async function neoCustomColor(){";
  js += "const color=document.getElementById('neoColor').value;";
  js += "const r=parseInt(color.substr(1,2),16);";
  js += "const g=parseInt(color.substr(3,2),16);";
  js += "const b=parseInt(color.substr(5,2),16);";
  js += "document.getElementById('neopixel-status').textContent=tr('apply_color','Appliquer couleur')+' RGB('+r+','+g+','+b+')...';";
  js += "const resp=await fetch('/api/neopixel-color?r='+r+'&g='+g+'&b='+b);";
  js += "const d=await resp.json();";
  js += "document.getElementById('neopixel-status').textContent=d.message;";
  js += "}";
  // FONCTIONS API - NeoPixel
  js += "async function testNeoPixel(){";
  js += "document.getElementById('neopixel-status').textContent=tr('testing','Test en cours...');";
  js += "const r=await fetch('/api/neopixel-test');const d=await r.json();";
  js += "document.getElementById('neopixel-status').textContent=d.result;";
  js += "}";
  js += "async function neoPattern(p){";
  js += "const r=await fetch('/api/neopixel-pattern?pattern='+p);const d=await r.json();";
  js += "document.getElementById('neopixel-status').textContent=d.message;";
  js += "}";

  // FONCTIONS API - TFT/OLED
  js += "async function testTFT(){";
  js += "document.getElementById('tft-status').textContent=tr('testing','Test en cours...')+' (15s)...';";
  js += "const r=await fetch('/api/tft-test');const d=await r.json();";
  js += "document.getElementById('tft-status').textContent=d.result;";
  js += "}";
  js += "async function tftPattern(p){";
  js += "const r=await fetch('/api/tft-pattern?pattern='+p);const d=await r.json();";
  js += "document.getElementById('tft-status').textContent=d.message;";
  js += "}";
  js += "async function testOLED(){";
  js += "document.getElementById('oled-status').textContent=tr('testing','Test en cours...')+' (25s)...';";
  js += "const r=await fetch('/api/oled-test');const d=await r.json();";
  js += "document.getElementById('oled-status').textContent=d.result;";
  js += "}";

  js += "async function oledPattern(p){";
  js += "document.getElementById('oled-status').textContent=tr('displaying','Affichage en cours...');";
  js += "const r=await fetch('/api/oled-pattern?pattern='+p);";
  js += "let msg=tr('command_sent','Commande envoyée');";
  js += "try{";
  js += "const d=await r.json();";
  js += "msg=d.message||d.result||msg;";
  js += "}catch(e){msg=tr('communication_error','Erreur de communication');}";
  js += "document.getElementById('oled-status').textContent=msg;";
  js += "}";

  // FONCTIONS API - Affichage texte TFT/OLED
  js += "async function tftDisplayText(){";
  js += "const text=document.getElementById('tftText').value;";
  js += "if(!text){alert('Entrez un texte!');return;}";
  js += "document.getElementById('tft-status').textContent=tr('displaying','Affichage en cours...');";
  js += "const r=await fetch('/api/tft-text?text='+encodeURIComponent(text));";
  js += "const d=await r.json();";
  js += "document.getElementById('tft-status').textContent=d.message;";
  js += "}";

  js += "async function oledDisplayText(){";
  js += "const text=document.getElementById('oledText').value;";
  js += "if(!text){alert('Entrez un message!');return;}";
  js += "document.getElementById('oled-status').textContent=tr('displaying','Affichage en cours...');";
  js += "const r=await fetch('/api/oled-message?message='+encodeURIComponent(text));";
  js += "const d=await r.json();";
  js += "document.getElementById('oled-status').textContent=d.message;";
  js += "}";

  js += "async function applyOledPins(){";
  js += "const sda=parseInt(document.getElementById('oled-sda').value,10);";
  js += "const scl=parseInt(document.getElementById('oled-scl').value,10);";
  js += "const status=document.getElementById('oled-status');";
  js += "status.textContent=tr('update_indicator','Mise à jour...');";
  js += "try{";
  js += "const r=await fetch('/api/oled-config?sda='+sda+'&scl='+scl);";
  js += "const d=await r.json();";
  js += "status.textContent=d.message||tr('command_sent','Commande envoyée');";
  js += "}catch(e){status.textContent=tr('communication_error','Erreur de communication');}";
  js += "}";

  // FONCTIONS API - Tests
  js += "async function testADC(){";
  js += "document.getElementById('adc-status').textContent=tr('testing','Test en cours...');";
  js += "const r=await fetch('/api/adc-test');const d=await r.json();";
  js += "let h='';d.readings.forEach(rd=>{h+='<div class=\"info-item\"><div class=\"info-label\">GPIO '+rd.pin+'</div><div class=\"info-value\">'+rd.raw+' ('+rd.voltage.toFixed(2)+'V)</div></div>';});";
  js += "document.getElementById('adc-results').innerHTML=h;";
  js += "document.getElementById('adc-status').textContent=d.result;";
  js += "}";

  js += "async function testAllGPIO(){";
  js += "document.getElementById('gpio-status').textContent=tr('testing','Test en cours...');";
  js += "const r=await fetch('/api/test-gpio');const d=await r.json();";
  js += "let h='';d.results.forEach(g=>{h+='<div class=\"gpio-item '+(g.working?'gpio-ok':'gpio-fail')+'\">GPIO '+g.pin+'<br>'+(g.working?'✅ OK':'❌ FAIL')+'</div>';});";
  js += "document.getElementById('gpio-results').innerHTML=h;";
  js += "document.getElementById('gpio-status').textContent='Terminé - '+d.results.length+' GPIO testés';";
  js += "}";

  js += "async function scanWiFi(){";
  js += "document.getElementById('wifi-status').textContent=tr('scanning','Scan en cours...');";
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
  js += "document.getElementById('cpu-bench').textContent=tr('testing','Test en cours...');";
  js += "document.getElementById('mem-bench').textContent=tr('testing','Test en cours...');";
  js += "const r=await fetch('/api/benchmark');const d=await r.json();";
  js += "document.getElementById('cpu-bench').textContent=d.cpu+' µs';";
  js += "document.getElementById('mem-bench').textContent=d.memory+' µs';";
  js += "}";

  js += "async function runStressTest(){";
  js += "document.getElementById('stress-status').textContent='⚠️ '+tr('testing','Test en cours...');";
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

  js += "function showUpdateIndicator(){document.getElementById('updateIndicator').classList.add('show');}";
  js += "function hideUpdateIndicator(){setTimeout(()=>document.getElementById('updateIndicator').classList.remove('show'),500);}";
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
  js += "async function changeLang(lang){";
  js += "if(lang===currentLang)return;";
  js += "const targetLang=lang;";
  js += "try{";
  js += "const response=await fetch('/api/set-language?lang='+encodeURIComponent(targetLang));";
  js += "const result=await response.json();";
  js += "if(!result.success)throw new Error(result.message||'');";
  js += "currentLang=targetLang;";
  js += "applyLanguageButtonState();";
  js += "translations=await fetchTranslations(targetLang);";
  js += "updateInterfaceTexts();";
  js += "await loadTab(activeTab,true);";
  js += "alert(tr('language_changed','Langue changée')+': '+targetLang.toUpperCase());";
  js += "}catch(error){console.error('Erreur changement langue',error);alert(tr('language_error','Erreur changement langue')+': '+error);}";
  js += "}";

  js += "function updateInterfaceTexts(){";
  js += "applyLanguageButtonState();";
  js += "document.documentElement.setAttribute('lang',currentLang);";
  js += "document.querySelectorAll('[data-i18n]').forEach(el=>{const key=el.dataset.i18n;const fallback=el.dataset.i18nFallback||el.textContent;el.textContent=tr(key,fallback);});";
  js += "const indicator=document.getElementById('updateIndicator');if(indicator)indicator.textContent=tr('update_indicator','Mise à jour...');";
  js += "}";

  return js;
}

#endif // WEB_INTERFACE_H
