/*
 * WEB_INTERFACE.H - Interface Web Dynamique v2.5.1
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
extern const char* const DIAGNOSTIC_SECURE_SCHEME;
extern const char* const DIAGNOSTIC_LEGACY_SCHEME;

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
  html += "color:#fff;padding:24px 18px 18px;text-align:center;position:relative}";
  html += ".header h1{font-size:2.2em;margin-bottom:6px;animation:fadeIn 0.5s}";
  html += ".header a{transition:opacity .2s}";
  html += ".header a.disabled{opacity:.6;pointer-events:none}";
  // --- [BUGFIX] Harmonisation des liens d'accès ---
  html += ".access-row{display:flex;flex-wrap:wrap;align-items:center;justify-content:center;gap:8px;font-size:.9em;opacity:.9;margin:6px 0}";
  html += ".access-label{font-weight:600}";
  html += ".access-link{color:#fff;text-decoration:underline}";
  html += ".access-link.disabled{opacity:.6;pointer-events:none}";
  html += ".access-sep{opacity:.7}";
  html += "@keyframes fadeIn{from{opacity:0;transform:translateY(-20px)}to{opacity:1;transform:translateY(0)}}";
  html += ".lang-switcher{position:absolute;top:16px;right:16px;display:flex;gap:5px}";
  html += ".lang-btn{padding:8px 15px;background:rgba(255,255,255,.2);border:2px solid rgba(255,255,255,.3);";
  html += "border-radius:5px;color:#fff;cursor:pointer;font-weight:bold;transition:all .3s}";
  html += ".lang-btn:hover{background:rgba(255,255,255,.3)}";
  html += ".lang-btn.active{background:rgba(255,255,255,.4);border-color:rgba(255,255,255,.6)}";
  html += ".status-indicator{display:inline-block;width:12px;height:12px;border-radius:50%;";
  html += "margin-right:8px;animation:pulse 2s infinite}";
  html += ".status-online{background:#0f0;box-shadow:0 0 10px #0f0}";
  html += ".status-offline{background:#f00;box-shadow:0 0 10px #f00}";
  html += "@keyframes pulse{0%,100%{opacity:1}50%{opacity:0.5}}";
  html += ".nav{display:flex;justify-content:center;gap:10px;margin-top:16px;flex-wrap:wrap}";
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
  html += ".status-live.success{background:#d4edda;color:#155724}";
  html += ".status-live.error{background:#f8d7da;color:#721c24}";
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
  html += "<button class='lang-btn active' onclick='changeLang(\"fr\",this)'>FR</button>";
  html += "<button class='lang-btn' onclick='changeLang(\"en\",this)'>EN</button>";
  html += "</div>";
  html += "<h1 id='main-title'>";
  html += "<span class='status-indicator status-online' id='statusIndicator'></span>";
  html += "Diagnostic ESP32 v";
  html += DIAGNOSTIC_VERSION_STR;
  html += "</h1>";
  html += "<div style='font-size:1.2em;margin:6px 0' id='chipModel'>";
  html += diagnosticData.chipModel;
  html += "</div>";
  // --- [BUGFIX] Suppression du doublon IP dans le bandeau ---
  bool ipAvailable = diagnosticData.ipAddress.length() > 0;
  String secureScheme = String(DIAGNOSTIC_SECURE_SCHEME);
  String legacyScheme = String(DIAGNOSTIC_LEGACY_SCHEME);
  String mdnsHost = String(MDNS_HOSTNAME_STR) + ".local";
  String mdnsLegacyUrl = legacyScheme + mdnsHost;
  // --- [BUGFIX] Affichage HTTP par défaut ---
  String mdnsDisplayUrl = mdnsLegacyUrl;
  String ipAccessHref = ipAvailable ? (legacyScheme + diagnosticData.ipAddress) : String("#");
  String ipLegacyHref = ipAvailable ? (legacyScheme + diagnosticData.ipAddress) : String();
  String ipAccessClass = String("access-link");
  if (!ipAvailable) {
    ipAccessClass += " disabled";
  }
  String ipAriaDisabled = ipAvailable ? String("false") : String("true");
  String ipLabelValue = ipAvailable ? ipAccessHref : String("IP indisponible");
  html += "<div class='access-row'>";
  html += "<span class='access-label'>Accès :</span>";
  // --- [NEW FEATURE] Liens HTTPS avec repli automatique ---
  html += "<a class='access-link' id='mdnsLink' href='";
  html += mdnsDisplayUrl;
  html += "' data-access-host='";
  html += mdnsHost;
  html += "' data-secure='";
  html += secureScheme;
  html += "' data-legacy='";
  html += legacyScheme;
  html += "' data-access-label='";
  html += mdnsHost;
  html += "' data-legacy-label='";
  html += mdnsLegacyUrl;
  html += "' data-label-id='mdnsAddressText' aria-disabled='false'><strong id='mdnsAddressText'>";
  html += mdnsDisplayUrl;
  html += "</strong></a>";
  html += "<span class='access-sep'>•</span>";
  html += "<a id='ipAddressLink' class='";
  html += ipAccessClass;
  html += "' href='";
  html += ipAccessHref;
  html += "' data-access-host='";
  html += (ipAvailable ? diagnosticData.ipAddress : String(""));
  html += "' data-secure='";
  html += secureScheme;
  html += "' data-legacy='";
  html += legacyScheme;
  html += "' data-access-label='";
  html += (ipAvailable ? diagnosticData.ipAddress : String(""));
  html += "' data-legacy-label='";
  html += ipLegacyHref;
  html += "' data-label-id='ipAddressText' aria-disabled='";
  html += ipAriaDisabled;
  html += "'><strong id='ipAddressText' data-placeholder='IP indisponible'>";
  html += ipLabelValue;
  html += "</strong></a></div>";
  html += "<div class='nav'>";
  html += "<button type='button' class='nav-btn active' data-tab='overview' data-i18n='nav_overview' onclick=\"showTab('overview',this);\">Vue d'ensemble</button>";
  html += "<button type='button' class='nav-btn' data-tab='leds' data-i18n='nav_leds' onclick=\"showTab('leds',this);\">LEDs</button>";
  html += "<button type='button' class='nav-btn' data-tab='screens' data-i18n='nav_screens' onclick=\"showTab('screens',this);\">Écrans</button>";
  html += "<button type='button' class='nav-btn' data-tab='tests' data-i18n='nav_tests' onclick=\"showTab('tests',this);\">Tests</button>";
  html += "<button type='button' class='nav-btn' data-tab='gpio' data-i18n='nav_gpio' onclick=\"showTab('gpio',this);\">GPIO</button>";
  html += "<button type='button' class='nav-btn' data-tab='wifi' data-i18n='nav_wireless' onclick=\"showTab('wifi',this);\">WiFi</button>";
  html += "<button type='button' class='nav-btn' data-tab='benchmark' data-i18n='nav_benchmark' onclick=\"showTab('benchmark',this);\">Performance</button>";
  html += "<button type='button' class='nav-btn' data-tab='export' data-i18n='nav_export' onclick=\"showTab('export',this);\">Export</button>";
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
  // --- [NEW FEATURE] Mise en cache des traductions dynamiques ---
  js += "let translationsCache={};";
  js += "function setTranslationsCache(t){translationsCache=t||{};}";
  js += "function tr(key,fr,en){const fallback=currentLang==='fr'?(fr||en||key):(en||fr||key);if(!translationsCache){return fallback;}");
  js += "const value=translationsCache[key];return typeof value==='string'?value:fallback;}";
  // --- [BUGFIX] Détection automatique HTTPS ---
  js += "const SECURE_PROBE_TIMEOUT=1500;";
  js += "const securePreferenceCache=typeof Map==='function'?new Map():null;";
  js += "const securePreferenceStore={};";
  js += "function getSecurePreference(key){if(securePreferenceCache){return securePreferenceCache.has(key)?securePreferenceCache.get(key):undefined;}return Object.prototype.hasOwnProperty.call(securePreferenceStore,key)?securePreferenceStore[key]:undefined;}";
  js += "function setSecurePreference(key,value){if(securePreferenceCache){securePreferenceCache.set(key,value);return;}securePreferenceStore[key]=value;}";
  js += "function probeSecureEndpoint(host,secure,cb){if(!host||typeof cb!=='function'){return;}const base=secure+host;let settled=false;const finalize=result=>{if(settled){return;}settled=true;clearTimeout(timer);cb(result);};const timer=setTimeout(()=>finalize(false),SECURE_PROBE_TIMEOUT);if(typeof fetch==='function'){fetch(base+'/',{mode:'no-cors'}).then(()=>finalize(true)).catch(()=>finalize(false));}else{try{const tester=new Image();tester.onload=function(){finalize(true);};tester.onerror=function(){finalize(false);};tester.src=base+'/favicon.ico?probe='+Date.now();}catch(err){finalize(false);}}}";
  js += "function shouldPreferSecureAccess(host,secure,legacy){if(!host||!secure||secure===legacy){return false;}const key=secure+host;const cached=getSecurePreference(key);if(typeof cached==='boolean'){return cached;}setSecurePreference(key,false);probeSecureEndpoint(host,secure,function(available){const previous=getSecurePreference(key);setSecurePreference(key,available);if(previous!==available){applyAccessLinkScheme();}});return false;}";
  js += "function applyAccessLinkScheme(){var links=document.querySelectorAll('[data-access-host]');for(var i=0;i<links.length;i++){var link=links[i];if(!link){continue;}var host=link.getAttribute('data-access-host');var secure=link.getAttribute('data-secure')||'https://';var legacy=link.getAttribute('data-legacy')||'http://';var labelId=link.getAttribute('data-label-id');var labelNode=labelId?document.getElementById(labelId):null;var labelHost=link.getAttribute('data-access-label')||host||'';var legacyLabel=link.getAttribute('data-legacy-label')||'';if(!host){link.href='#';link.classList.add('disabled');link.setAttribute('aria-disabled','true');if(labelNode&&labelNode.getAttribute('data-placeholder')){labelNode.textContent=labelNode.getAttribute('data-placeholder');}continue;}var useSecure=shouldPreferSecureAccess(host,secure,legacy);var scheme=useSecure?secure:legacy;link.href=scheme+host;link.classList.remove('disabled');link.setAttribute('aria-disabled','false');if(labelNode){if(!useSecure&&legacyLabel){labelNode.textContent=legacyLabel;}else{labelNode.textContent=scheme+labelHost;}}}}";

  // Initialisation - CORRIGÉE
  // --- [BUGFIX] Navigation onglets : délégation dès le chargement ---
  js += "document.addEventListener('DOMContentLoaded',function(){";
  js += "console.log('Interface chargée');";
  // --- [NEW FEATURE] Préchargement des traductions UI dynamiques ---
  js += "fetch('/api/get-translations').then(r=>r.json()).then(t=>{setTranslationsCache(t);updateInterfaceTexts();}).catch(err=>c";
  js += "onsole.warn('Translations unavailable',err));";
  js += "initNavigation();";
  js += "applyAccessLinkScheme();";
  js += "loadAllData();";
  js += "startAutoUpdate();";
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
  // --- [BUGFIX] Synchronisation du lien IP dynamique ---
  js += "const chipModelEl=document.getElementById('chipModel');";
  js += "if(chipModelEl){chipModelEl.textContent=d.chipModel||'';}";
  js += "const ipLabel=document.getElementById('ipAddressText');";
  js += "const ipLink=document.getElementById('ipAddressLink');";
  js += "const hasIp=d.ipAddress&&d.ipAddress.length;";
  js += "const secureScheme=(ipLink&&ipLink.getAttribute('data-secure'))||'https://';";
  js += "const legacyScheme=(ipLink&&ipLink.getAttribute('data-legacy'))||'http://';";
  js += "// --- [BUGFIX] Réalignement HTTP/HTTPS dynamique ---\n";
  js += "if(ipLabel){if(hasIp){ipLabel.textContent=legacyScheme+d.ipAddress;}else{var placeholder=ipLabel.getAttribute('data-placeholder')||'IP indisponible';ipLabel.textContent=placeholder;}}";
  js += "if(ipLink){if(hasIp){ipLink.href=legacyScheme+d.ipAddress;ipLink.setAttribute('data-access-host',d.ipAddress);ipLink.setAttribute('data-access-label',d.ipAddress);ipLink.setAttribute('data-legacy-label',legacyScheme+d.ipAddress);ipLink.setAttribute('aria-disabled','false');ipLink.classList.remove('disabled');}else{ipLink.href='#';ipLink.setAttribute('data-access-host','');ipLink.setAttribute('data-access-label','');ipLink.setAttribute('data-legacy-label','');ipLink.setAttribute('aria-disabled','true');ipLink.classList.add('disabled');}}";
  js += "applyAccessLinkScheme();";
  js += "}";
  js += "async function updateMemoryInfo(){await fetch('/api/memory');}";
  js += "async function updateWiFiInfo(){await fetch('/api/wifi-info');}";
  js += "async function updatePeripheralsInfo(){await fetch('/api/peripherals');}";

  // --- [BUGFIX] Navigation onglets : délégation universelle ---
  js += "function showTab(tabName,btn){";
  js += "var contents=document.querySelectorAll('.tab-content');";
  js += "for(var i=0;i<contents.length;i++){contents[i].classList.remove('active');}";
  js += "var tab=document.getElementById(tabName);";
  js += "if(tab){tab.classList.add('active');}else{loadTab(tabName);}";
  js += "setActiveTabButton(tabName,btn);";
  js += "}";

  js += "function setActiveTabButton(tabName,btn){";
  js += "var buttons=document.querySelectorAll('.nav-btn');";
  js += "for(var i=0;i<buttons.length;i++){buttons[i].classList.remove('active');}";
  js += "if(btn&&btn.classList){btn.classList.add('active');return;}";
  js += "var selector='.nav-btn[data-tab=\"'+tabName+'\"]';";
  js += "var fallback=document.querySelector(selector);";
  js += "if(fallback){fallback.classList.add('active');}";
  js += "}";

  js += "function findNavButton(el){";
  js += "while(el&&el.classList&&!el.classList.contains('nav-btn')){el=el.parentElement;}";
  js += "if(el&&el.classList&&el.classList.contains('nav-btn')){return el;}";
  js += "return null;";
  js += "}";

  js += "function initNavigation(){";
  js += "var navs=document.querySelectorAll('.nav');";
  js += "if(!navs||!navs.length){showTab('overview');return;}";
  js += "for(var n=0;n<navs.length;n++){(function(nav){";
  js += "nav.addEventListener('click',function(e){";
  js += "var btn=findNavButton(e.target);";
  js += "if(!btn){return;}";
  js += "e.preventDefault();";
  js += "var target=btn.getAttribute('data-tab');";
  js += "if(target){showTab(target,btn);}";
  js += "});";
  js += "})(navs[n]);}";
  js += "var active=document.querySelector('.nav-btn.active');";
  js += "if(!active){var list=document.querySelectorAll('.nav-btn');if(list.length>0){active=list[0];}}";
  js += "if(active){showTab(active.getAttribute('data-tab'),active);}else{showTab('overview');}";
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
  js += "updateInterfaceTexts();";
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
  js += "let h='<div class=\"section\"><h2>💡 LED Intégrée</h2><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">GPIO</div><div class=\"info-value\">GPIO '+d.builtin.pin+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">Statut</div><div class=\"info-value\" id=\"builtin-led-status\">'+d.builtin.status+'</div></div>';";
  js += "h+='<div class=\"info-item\" style=\"grid-column:1/-1;text-align:center\">';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"testBuiltinLED()\">🧪 Test complet</button> ';";
  js += "h+='<button class=\"btn btn-success\" onclick=\"ledBlink()\">⚡ Clignoter</button> ';";
  js += "h+='<button class=\"btn btn-info\" onclick=\"ledFade()\">🌊 Fade</button> ';";
  js += "h+='<button class=\"btn btn-warning\" onclick=\"ledOn()\">💡 Allumer</button> ';";
  js += "h+='<button class=\"btn btn-danger\" onclick=\"ledOff()\">⭕ Éteindre</button>';";
  js += "h+='</div></div></div>';";

  js += "h+='<div class=\"section\"><h2>🌈 NeoPixel / WS2812B</h2><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">GPIO</div><div class=\"info-value\">GPIO '+d.neopixel.pin+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">Nombre de LEDs</div><div class=\"info-value\">'+d.neopixel.count+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">Statut</div><div class=\"info-value\" id=\"neopixel-status\">'+d.neopixel.status+'</div></div>';";
  js += "h+='<div class=\"info-item\" style=\"grid-column:1/-1;text-align:center\">';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"testNeoPixel()\">🧪 Test complet</button><br><br>';";

  // Patterns
  js += "h+='<strong>Animations :</strong><br>';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"neoPattern(\\'rainbow\\')\">🌈 Arc-en-ciel</button> ';";
  js += "h+='<button class=\"btn btn-success\" onclick=\"neoPattern(\\'blink\\')\">⚡ Clignoter</button> ';";
  js += "h+='<button class=\"btn btn-info\" onclick=\"neoPattern(\\'fade\\')\">🌊 Fade</button> ';";
  js += "h+='<button class=\"btn btn-warning\" onclick=\"neoPattern(\\'chase\\')\">🏃 Chase</button><br><br>';";

  // Sélecteur de couleur
  js += "h+='<strong>Couleur personnalisée :</strong><br>';";
  js += "h+='<input type=\"color\" id=\"neoColor\" value=\"#ff0000\" style=\"height:50px;width:120px;border:none;border-radius:5px;cursor:pointer\"> ';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"neoCustomColor()\">🎨 Appliquer couleur</button><br><br>';";

  js += "h+='<button class=\"btn btn-danger\" onclick=\"neoPattern(\\'off\\')\">⭕ Tout éteindre</button>';";
  js += "h+='</div></div></div>';";
  js += "return h;";
  js += "}";

  // --- [NEW FEATURE] Traduction complète des interactions OLED ---
  js += "function buildScreens(d){";
  js += "const rotation=(typeof d.oled.rotation!=='undefined')?d.oled.rotation:0;";
  js += "const hasOled=d&&d.oled&&((typeof d.oled.available==='undefined')?true:!!d.oled.available);";
  js += "let h='<div class=\\"section\\"><h2 data-i18n=\\"oled_screen\\" data-i18n-prefix=\\"🖥️ \">'+tr('oled_screen','Écran OLED 0.96\\\\\" I2C','OLED Screen 0.96\\\\\" I2C')+'</h2><div class=\\"info-grid\\">';";
  js += "h+='<div class=\\"info-item\\"><div class=\\"info-label\\" data-i18n=\\"status\\">'+tr('status','Statut','Status')+'</div><div class=\\"info-value\\" id=\\"oled-status\\">'+d.oled.status+'</div></div>';";
  js += "h+='<div class=\\"info-item\\"><div class=\\"info-label\\" data-i18n=\\"i2c_pins\\">'+tr('i2c_pins','Pins I2C','I2C Pins')+'</div><div class=\\"info-value\\" id=\\"oled-pins\\">SDA:'+d.oled.pins.sda+' SCL:'+d.oled.pins.scl+'</div></div>';";
  js += "h+='<div class=\\"info-item\\"><div class=\\"info-label\\" data-i18n=\\"rotation\\">'+tr('rotation','Rotation','Rotation')+'</div><div class=\\"info-value\\" id=\\"oled-rotation-display\\">'+rotation+'</div></div>';";
  js += "h+='<div class=\\"info-item\\" style=\\"grid-column:1/-1;text-align:center\\">';";
  js += "h+='SDA: <input type=\\"number\\" id=\\"oledSDA\\" value=\\"'+d.oled.pins.sda+'\\" min=\\"0\\" max=\\"48\\" style=\\"width:70px\\"> ';";
  js += "h+='SCL: <input type=\\"number\\" id=\\"oledSCL\\" value=\\"'+d.oled.pins.scl+'\\" min=\\"0\\" max=\\"48\\" style=\\"width:70px\\"> ';";
  js += "h+=tr('rotation','Rotation','Rotation')+': <select id=\\"oledRotation\\" style=\\"width:90px;padding:10px;border:2px solid #ddd;border-radius:5px\\">';";
  js += "for(let i=0;i<4;i++){h+='<option value=\\\\''+i+'\\\\''+(i===rotation?' selected':'')+'>'+i+'</option>';};";
  js += "h+='</select> ';";
  js += "h+='<button class=\\"btn btn-info\\" data-i18n=\\"apply_redetect\\" data-i18n-prefix=\\"🔄 \\" onclick=\\"configOLED()\\">'+tr('apply_redetect','Appliquer et Re-détecter','Apply and Re-detect')+'</button>';";
  js += "if(hasOled){";
  js += "h+='<div style=\\"margin-top:15px\\"><button class=\\"btn btn-primary\\" data-i18n=\\"full_test\\" data-i18n-prefix=\\"🧪 \\" data-i18n-suffix=\\" (25s)\\" onclick=\\"testOLED()\\">'+tr('full_test','Test Complet','Full Test')+'</button></div>';";
  js += "h+='<div class=\\"oled-step-grid\\" style=\\"margin-top:15px;display:grid;grid-template-columns:repeat(auto-fit,minmax(180px,1fr));gap:10px\\">';";
  js += "h+='<button class=\\"btn btn-secondary\\" data-i18n=\\"oled_step_welcome\\" data-i18n-prefix=\\"🏁 \\" onclick=\\"oledStep(\\'welcome\\')\\">'+tr('oled_step_welcome','Accueil','Welcome')+'</button>';";
  js += "h+='<button class=\\"btn btn-secondary\\" data-i18n=\\"oled_step_big_text\\" data-i18n-prefix=\\"🔠 \\" onclick=\\"oledStep(\\'big_text\\')\\">'+tr('oled_step_big_text','Texte grand format','Large text')+'</button>';";
  js += "h+='<button class=\\"btn btn-secondary\\" data-i18n=\\"oled_step_text_sizes\\" data-i18n-prefix=\\"🔤 \\" onclick=\\"oledStep(\\'text_sizes\\')\\">'+tr('oled_step_text_sizes','Tailles de texte','Text sizes')+'</button>';";
  js += "h+='<button class=\\"btn btn-secondary\\" data-i18n=\\"oled_step_shapes\\" data-i18n-prefix=\\"🟦 \\" onclick=\\"oledStep(\\'shapes\\')\\">'+tr('oled_step_shapes','Formes géométriques','Geometric shapes')+'</button>';";
  js += "h+='<button class=\\"btn btn-secondary\\" data-i18n=\\"oled_step_horizontal_lines\\" data-i18n-prefix=\\"📏 \\" onclick=\\"oledStep(\\'horizontal_lines\\')\\">'+tr('oled_step_horizontal_lines','Lignes horizontales','Horizontal lines')+'</button>';";
  js += "h+='<button class=\\"btn btn-secondary\\" data-i18n=\\"oled_step_diagonals\\" data-i18n-prefix=\\"📐 \\" onclick=\\"oledStep(\\'diagonals\\')\\">'+tr('oled_step_diagonals','Lignes diagonales','Diagonal lines')+'</button>';";
  js += "h+='<button class=\\"btn btn-secondary\\" data-i18n=\\"oled_step_moving_square\\" data-i18n-prefix=\\"⬜ \\" onclick=\\"oledStep(\\'moving_square\\')\\">'+tr('oled_step_moving_square','Carré en mouvement','Moving square')+'</button>';";
  js += "h+='<button class=\\"btn btn-secondary\\" data-i18n=\\"oled_step_progress_bar\\" data-i18n-prefix=\\"📊 \\" onclick=\\"oledStep(\\'progress_bar\\')\\">'+tr('oled_step_progress_bar','Barre de progression','Progress bar')+'</button>';";
  js += "h+='<button class=\\"btn btn-secondary\\" data-i18n=\\"oled_step_scroll_text\\" data-i18n-prefix=\\"📜 \\" onclick=\\"oledStep(\\'scroll_text\\')\\">'+tr('oled_step_scroll_text','Texte défilant','Scrolling text')+'</button>';";
  js += "h+='<button class=\\"btn btn-secondary\\" data-i18n=\\"oled_step_final_message\\" data-i18n-prefix=\\"✅ \\" onclick=\\"oledStep(\\'final_message\\')\\">'+tr('oled_step_final_message','Message final','Final message')+'</button>';";
  js += "h+='</div>';";
  js += "h+='<div style=\\"margin-top:15px\\">';";
  js += "h+='<input type=\\"text\\" id=\\"oledText\\" data-i18n-placeholder=\\"custom_message\\" placeholder=\\"'+tr('custom_message','Message personnalisé','Custom message')+'\\" style=\\"width:300px;padding:10px\\"> ';";
  js += "h+='<button class=\\"btn btn-success\\" data-i18n=\\"show_message\\" data-i18n-prefix=\\"📤 \\" onclick=\\"oledDisplayText()\\">'+tr('show_message','Afficher Message','Display Message')+'</button>';";
  js += "h+='</div>';";
  js += "}";
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
  js += "let h='<div class=\"section\"><h2>🔌 Test GPIO</h2>';";
  js += "h+='<div style=\"text-align:center;margin:20px 0\"><button class=\"btn btn-primary\" onclick=\"testAllGPIO()\">🧪 Tester tous les GPIO</button>';";
  js += "h+='<div id=\"gpio-status\" class=\"status-live\">Cliquez pour tester</div></div>';";
  js += "h+='<div id=\"gpio-results\" class=\"gpio-grid\"></div></div>';";
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
  js += "async function testBuiltinLED(){";
  js += "setStatus('builtin-led-status','Test en cours...',null);";
  js += "const r=await fetch('/api/builtin-led-test');const d=await r.json();";
  js += "setStatus('builtin-led-status',d.result,d.success?'success':'error');";
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
  js += "document.getElementById('neopixel-status').textContent='Application couleur RGB('+r+','+g+','+b+')...';";
  js += "const resp=await fetch('/api/neopixel-color?r='+r+'&g='+g+'&b='+b);";
  js += "const d=await resp.json();";
  js += "document.getElementById('neopixel-status').textContent=d.message;";
  js += "}";
  // FONCTIONS API - NeoPixel
  js += "async function testNeoPixel(){";
  js += "document.getElementById('neopixel-status').textContent='Test en cours...';";
  js += "const r=await fetch('/api/neopixel-test');const d=await r.json();";
  js += "document.getElementById('neopixel-status').textContent=d.result;";
  js += "}";
  js += "async function neoPattern(p){";
  js += "const r=await fetch('/api/neopixel-pattern?pattern='+p);const d=await r.json();";
  js += "document.getElementById('neopixel-status').textContent=d.message;";
  js += "}";

  // FONCTIONS API - OLED
  js += "async function testOLED(){";
  js += "document.getElementById('oled-status').textContent='Test en cours (25s)...';";
  js += "const r=await fetch('/api/oled-test');const d=await r.json();";
  js += "document.getElementById('oled-status').textContent=d.result;";
  js += "}";
  js += "async function oledStep(step){";
  js += "setStatus('oled-status','Étape en cours...',null);";
  js += "try{const r=await fetch('/api/oled-step?step='+step);const d=await r.json();setStatus('oled-status',d.message,d.success?'success':'error');}catch(e){setStatus('oled-status','Erreur: '+e,'error');}";
  js += "}";

  // FONCTIONS API - Affichage texte OLED
  js += "async function oledDisplayText(){";
  js += "const text=document.getElementById('oledText').value;";
  js += "if(!text){setStatus('oled-status','Message requis','error');return;}";
  js += "setStatus('oled-status','Affichage en cours...',null);";
  js += "try{const r=await fetch('/api/oled-message?message='+encodeURIComponent(text));const d=await r.json();setStatus('oled-status',d.message,d.success?'success':'error');}catch(e){setStatus('oled-status','Erreur: '+e,'error');}";
  js += "}";
  js += "async function configOLED(){";
  js += "setStatus('oled-status','Reconfiguration...',null);";
  js += "const sda=document.getElementById('oledSDA').value;";
  js += "const scl=document.getElementById('oledSCL').value;";
  js += "const rotation=document.getElementById('oledRotation').value;";
  js += "try{";
  js += "const r=await fetch('/api/oled-config?sda='+sda+'&scl='+scl+'&rotation='+rotation);";
  js += "const d=await r.json();";
  js += "setStatus('oled-status',d.message||'Configuration invalide',d.success?'success':'error');";
  js += "if(d.success){if(typeof d.sda!=='undefined'){document.getElementById('oledSDA').value=d.sda;document.getElementById('oledSCL').value=d.scl;}";
  js += "const pins=document.getElementById('oled-pins');if(pins){pins.textContent='SDA:'+d.sda+' SCL:'+d.scl;}const rotDisplay=document.getElementById('oled-rotation-display');if(rotDisplay){rotDisplay.textContent=d.rotation;}if(document.getElementById('oledRotation')){document.getElementById('oledRotation').value=d.rotation;}}";
  js += "}catch(e){setStatus('oled-status','Erreur: '+e,'error');}";
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

  js += "function showUpdateIndicator(){document.getElementById('updateIndicator').classList.add('show');}";
  js += "function hideUpdateIndicator(){setTimeout(()=>document.getElementById('updateIndicator').classList.remove('show'),500);}";
  js += "function updateStatusIndicator(c){";
  js += "const i=document.getElementById('statusIndicator');";
  js += "if(c){i.classList.remove('status-offline');i.classList.add('status-online');}";
  js += "else{i.classList.remove('status-online');i.classList.add('status-offline');}";
  js += "}";

  js += "function setStatus(id,text,state){const el=document.getElementById(id);if(!el)return;el.textContent=text;el.classList.remove('success','error');if(state){el.classList.add(state);}}";

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
  js += "function changeLang(lang,btn){";
  js += "fetch('/api/set-language?lang='+lang).then(r=>r.json()).then(d=>{";
  js += "if(d.success){";
  js += "currentLang=lang;";
  js += "document.querySelectorAll('.lang-btn').forEach(b=>b.classList.remove('active'));";
  js += "if(btn){btn.classList.add('active');}else{document.querySelectorAll('.lang-btn').forEach(b=>{if(b.textContent.toLowerCase()===lang)b.classList.add('active');});}";
  js += "return fetch('/api/get-translations');";
  js += "}}").then(r=>r.json()).then(translations=>{";
  js += "setTranslationsCache(translations);";
  js += "updateInterfaceTexts();";
  js += "const ind=document.getElementById('updateIndicator');if(ind){ind.textContent='Langue changée : '+lang.toUpperCase();showUpdateIndicator();hideUpdateIndicator();}";
  js += "}).catch(e=>{const ind=document.getElementById('updateIndicator');if(ind){ind.textContent='Erreur changement langue: '+e;showUpdateIndicator();hideUpdateIndicator();}});";
  js += "}";

  js += "function updateInterfaceTexts(t){";
  js += "if(t){setTranslationsCache(t);}";
  js += "const translations=translationsCache||{};";
  // --- [NEW FEATURE] Actualisation complète des libellés traduits ---
  js += "document.querySelectorAll('[data-i18n]').forEach(el=>{const key=el.getAttribute('data-i18n');if(!key){return;}const value=translations[key];if(typeof value!=='string'){return;}const prefix=el.getAttribute('data-i18n-prefix')||'';const suffix=el.getAttribute('data-i18n-suffix')||'';if(el.tagName==='INPUT'){if(el.hasAttribute('placeholder')){el.setAttribute('placeholder',prefix+value+suffix);}else{el.value=prefix+value+suffix;}}else{el.textContent=prefix+value+suffix;}});";
  js += "document.querySelectorAll('[data-i18n-placeholder]').forEach(el=>{const key=el.getAttribute('data-i18n-placeholder');const value=translations[key];if(typeof value!=='string'){return;}const prefix=el.getAttribute('data-i18n-prefix')||'';const suffix=el.getAttribute('data-i18n-suffix')||'';el.setAttribute('placeholder',prefix+value+suffix);});";
  js += "}";

  return js;
}

#endif // WEB_INTERFACE_H
