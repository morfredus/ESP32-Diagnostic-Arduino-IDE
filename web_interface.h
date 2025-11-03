/*
 * WEB_INTERFACE.H - Interface Web Dynamique v3.5.1
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
String htmlEscape(const String& raw);
String buildTranslationsJSON();
String generateHTML();
String generateJavaScript();

// Implémentation de handleJavaScript()
void handleJavaScript() {
  server.send(200, "application/javascript; charset=utf-8", generateJavaScript());
}

// Génère le HTML principal
String generateHTML() {
  const char* langCode = (currentLanguage == LANG_EN) ? "en" : "fr";
  String html;
  html.reserve(9500);  // Reserve memory to avoid reallocations
  html = "<!DOCTYPE html><html lang='";
  html += langCode;
  html += "'><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>";
  html += htmlEscape(T().title.str());
  html += " ";
  html += htmlEscape(T().version.str());
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
  html += "<div class='update-indicator' id='updateIndicator' data-i18n='updating'>";
  html += htmlEscape(T().updating.str());
  html += "</div>";
  html += "<div class='container'>";
  html += "<div class='header'>";
  html += "<div class='lang-switcher' role='group' aria-label='";
  html += htmlEscape(T().language_label.str());
  html += "'>";
  html += "<button class='lang-btn";
  if (currentLanguage == LANG_FR) {
    html += " active";
  }
  html += "' data-lang='fr' onclick='changeLang(\"fr\",this)'>FR</button>";
  html += "<button class='lang-btn";
  if (currentLanguage == LANG_EN) {
    html += " active";
  }
  html += "' data-lang='en' onclick='changeLang(\"en\",this)'>EN</button>";
  html += "</div>";
  html += "<h1 id='main-title'>";
  html += "<span class='status-indicator status-online' id='statusIndicator'></span>";
  html += htmlEscape(T().title.str());
  html += " ";
  html += htmlEscape(T().version.str());
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
  String ipLabelValue = ipAvailable ? ipAccessHref : htmlEscape(String(T().ip_unavailable.str()));
  html += "<div class='access-row'>";
  html += "<span class='access-label' data-i18n='access' data-i18n-suffix=' :'>";
  html += htmlEscape(T().access.str());
  html += "</span>";
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
  html += "'><strong id='ipAddressText' data-placeholder='";
  html += htmlEscape(T().ip_unavailable.str());
  html += "' data-placeholder-key='ip_unavailable'>";
  html += ipLabelValue;
  html += "</strong></a></div>";
  // --- [REORGANIZATION] Navigation ergonomique par thème fonctionnel ---
  html += "<div class='nav'>";
  html += "<button type='button' class='nav-btn active' data-tab='overview' data-i18n='nav_overview' onclick=\"showTab('overview',this);\">";
  html += htmlEscape(T().nav_overview.str());
  html += "</button>";
  html += "<button type='button' class='nav-btn' data-tab='display-signal' data-i18n='nav_display_signal' onclick=\"showTab('display-signal',this);\">";
  html += htmlEscape(T().nav_display_signal.str());
  html += "</button>";
  html += "<button type='button' class='nav-btn' data-tab='sensors' data-i18n='nav_sensors' onclick=\"showTab('sensors',this);\">";
  html += htmlEscape(T().nav_sensors.str());
  html += "</button>";
  html += "<button type='button' class='nav-btn' data-tab='hardware-tests' data-i18n='nav_hardware_tests' onclick=\"showTab('hardware-tests',this);\">";
  html += htmlEscape(T().nav_hardware_tests.str());
  html += "</button>";
  html += "<button type='button' class='nav-btn' data-tab='wireless' data-i18n='nav_wireless' onclick=\"showTab('wireless',this);\">";
  html += htmlEscape(T().nav_wireless.str());
  html += "</button>";
  html += "<button type='button' class='nav-btn' data-tab='benchmark' data-i18n='nav_benchmark' onclick=\"showTab('benchmark',this);\">";
  html += htmlEscape(T().nav_benchmark.str());
  html += "</button>";
  html += "<button type='button' class='nav-btn' data-tab='export' data-i18n='nav_export' onclick=\"showTab('export',this);\">";
  html += htmlEscape(T().nav_export.str());
  html += "</button>";
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
  String js;
  js.reserve(45000);  // Reserve memory to avoid reallocations (~42KB final size)
  js = "console.log('ESP32 Diagnostic v";
  js += DIAGNOSTIC_VERSION_STR;
  js += " - Initialisation');";
  js += "const UPDATE_INTERVAL=5000;";
  js += "let currentLang='";
  js += (currentLanguage == LANG_EN) ? "en" : "fr";
  js += "';";
  js += "let updateTimer=null;";
  js += "let isConnected=true;";
  // --- [NEW FEATURE] Catalogue de traductions préchargé ---
  js += "const DEFAULT_TRANSLATIONS=";
  js += buildTranslationsJSON();
  js += ";";
  js += "let translationsCache=DEFAULT_TRANSLATIONS;";
  js += "function getCurrentTranslations(){return translationsCache||DEFAULT_TRANSLATIONS;}";
  js += "function setTranslationsCache(t){if(t&&typeof t==='object'){translationsCache=Object.assign({},DEFAULT_TRANSLATIONS,t);}";
  js += "else{translationsCache=DEFAULT_TRANSLATIONS;}}";
  js += "function fetchTranslations(){const cacheBypass='ts='+(Date.now());const endpoint='/api/get-translations?'+cacheBypass;return fetch(endpoint,{cache:'no-store'}).then(r=>r.json());}";
  js += "function tr(key){const translations=getCurrentTranslations();const source=translations&&translations[key];const fallback=DEFAULT_TRANSLATIONS[key];return typeof source==='string'?source:(typeof fallback==='string'?fallback:key);}";
  js += "function clearTranslationAttributes(el){if(!el)return;el.removeAttribute('data-i18n');el.removeAttribute('data-i18n-prefix');el.removeAttribute('data-i18n-suffix');if(el.attributes){const toRemove=[];for(let i=0;i<el.attributes.length;i++){const name=el.attributes[i].name;if(name&&name.indexOf('data-i18n-replace-')===0){toRemove.push(name);}}toRemove.forEach(attr=>el.removeAttribute(attr));}}";
  js += "function collectReplacementAttributes(el){const replacements={};if(!el||!el.attributes){return replacements;}for(let i=0;i<el.attributes.length;i++){const attr=el.attributes[i];if(!attr||!attr.name||attr.name.indexOf('data-i18n-replace-')!==0){continue;}const key=attr.name.substring(18);if(key){replacements[key]=attr.value;}}return replacements;}";
  js += "function resolveTranslationValue(key,el,translations){if(!key){return undefined;}const source=translations&&translations[key];const fallback=DEFAULT_TRANSLATIONS[key];let value=typeof source==='string'?source:(typeof fallback==='string'?fallback:undefined);if(typeof value!=='string'){return undefined;}const replacements=collectReplacementAttributes(el);Object.keys(replacements).forEach(name=>{const raw=replacements[name];if(typeof raw==='undefined'){return;}const text=String(raw);const lower='{'+name+'}';const upper='{'+name.toUpperCase()+'}';const percent='%'+name.toUpperCase()+'%';value=value.split(lower).join(text);value=value.split(upper).join(text);value=value.split(percent).join(text);});return value;}";
  js += "function translateElement(el,translations){if(!el){return;}const key=el.getAttribute('data-i18n');if(!key){return;}const value=resolveTranslationValue(key,el,translations);if(typeof value!=='string'){return;}const prefix=el.getAttribute('data-i18n-prefix')||'';const suffix=el.getAttribute('data-i18n-suffix')||'';el.textContent=prefix+value+suffix;}";
  js += "function applyPlaceholderTranslation(el,translations){if(!el){return;}const key=el.getAttribute('data-i18n-placeholder');if(!key){return;}const value=resolveTranslationValue(key,el,translations);if(typeof value!=='string'){return;}const prefix=el.getAttribute('data-i18n-prefix')||'';const suffix=el.getAttribute('data-i18n-suffix')||'';el.setAttribute('placeholder',prefix+value+suffix);}";
  js += "function setElementTranslation(el,config){if(!el){return;}if(!config||typeof config.key!=='string'){clearTranslationAttributes(el);if(config&&typeof config.text==='string'){el.textContent=config.text;}return;}clearTranslationAttributes(el);el.setAttribute('data-i18n',config.key);if(Object.prototype.hasOwnProperty.call(config,'prefix')){if(config.prefix){el.setAttribute('data-i18n-prefix',config.prefix);}else{el.removeAttribute('data-i18n-prefix');}}if(Object.prototype.hasOwnProperty.call(config,'suffix')){if(config.suffix){el.setAttribute('data-i18n-suffix',config.suffix);}else{el.removeAttribute('data-i18n-suffix');}}if(el.attributes){const toRemove=[];for(let i=0;i<el.attributes.length;i++){const name=el.attributes[i].name;if(name&&name.indexOf('data-i18n-replace-')===0){toRemove.push(name);}}toRemove.forEach(attr=>el.removeAttribute(attr));}if(config.replacements&&typeof config.replacements==='object'){Object.keys(config.replacements).forEach(name=>{if(typeof name!=='string'){return;}el.setAttribute('data-i18n-replace-'+name,String(config.replacements[name]));});}translateElement(el,getCurrentTranslations());}";
  js += "function updatePlaceholderAttributes(translations){document.querySelectorAll('[data-placeholder-key]').forEach(el=>{const key=el.getAttribute('data-placeholder-key');if(!key){return;}const value=resolveTranslationValue(key,el,translations);if(typeof value==='string'){el.setAttribute('data-placeholder',value);}});}";
  js += "function updateInterfaceTexts(t){if(t){setTranslationsCache(t);}const translations=getCurrentTranslations();document.querySelectorAll('[data-i18n]').forEach(el=>translateElement(el,translations));document.querySelectorAll('[data-i18n-placeholder]').forEach(el=>applyPlaceholderTranslation(el,translations));updatePlaceholderAttributes(translations);}";
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
  js += "console.log('ESP32 Diagnostic - '+tr('interface_loaded'));";
  js += "document.documentElement.setAttribute('lang',currentLang);";
  // --- [NEW FEATURE] Préchargement des traductions UI dynamiques ---
  js += "fetchTranslations().then(t=>{setTranslationsCache(t);updateInterfaceTexts();}).catch(err=>console.warn('Translations unavailable',err));";
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
  js += "if(ipLabel){if(hasIp){clearTranslationAttributes(ipLabel);ipLabel.textContent=legacyScheme+d.ipAddress;}else{ipLabel.setAttribute('data-i18n','ip_unavailable');translateElement(ipLabel,getCurrentTranslations());}}";
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
  js += "tab.innerHTML='<div class=\"section\"><div class=\"loading\"></div><p style=\"text-align:center\" data-i18n=\"loading\">'+tr('loading')+'</p></div>';";
  js += "tab.classList.add('active');";
  js += "try{";
  js += "if(tabName==='overview'){const r=await fetch('/api/overview');const d=await r.json();tab.innerHTML=buildOverview(d);}";
  js += "else if(tabName==='display-signal'){const leds=await fetch('/api/leds-info');const screens=await fetch('/api/screens-info');const ld=await leds.json();const sd=await screens.json();tab.innerHTML=buildDisplaySignal(ld,sd);}";
  js += "else if(tabName==='sensors'){tab.innerHTML=buildSensors();}";
  js += "else if(tabName==='hardware-tests'){tab.innerHTML=buildHardwareTests();}";
  js += "else if(tabName==='wireless'){tab.innerHTML=buildWireless();await refreshBluetoothStatus();}";
  js += "else if(tabName==='benchmark'){tab.innerHTML=buildBenchmark();}";
  js += "else if(tabName==='export'){tab.innerHTML=buildExport();}";
  js += "updateInterfaceTexts();";
  js += "}catch(e){tab.innerHTML='<div class=\"section\"><h2 data-i18n=\"error_label\" data-i18n-prefix=\"❌\">'+tr('error_label')+'</h2><p>'+String(e)+'</p></div>';updateInterfaceTexts();}";
  js += "}";

    // Build Overview - localisation complète
  js += "function buildOverview(d){";
  js += "let h='<div class=\"section\"><h2 data-i18n=\"chip_info\" data-i18n-prefix=\"🔧\">'+tr('chip_info')+'</h2><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"full_model\">'+tr('full_model')+'</div><div class=\"info-value\">'+d.chip.model+' <span data-i18n=\"revision\">'+tr('revision')+'</span> '+d.chip.revision+'</div></div>';";
  js += "const cpuSummary=d.chip.cores+' <span data-i18n=\"cores\">'+tr('cores')+'</span> @ '+d.chip.freq+' MHz';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"cpu_cores\">'+tr('cpu_cores')+'</div><div class=\"info-value\">'+cpuSummary+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"mac_wifi\">'+tr('mac_wifi')+'</div><div class=\"info-value\">'+d.chip.mac+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"uptime\">'+tr('uptime')+'</div><div class=\"info-value\" id=\"uptime\">'+formatUptime(d.chip.uptime)+'</div></div>';";
  js += "if(d.chip.temperature!==-999){h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"cpu_temp\">'+tr('cpu_temp')+'</div><div class=\"info-value\" id=\"temperature\">'+d.chip.temperature.toFixed(1)+' °C</div></div>'; }";
  js += "h+='</div></div>';";

  js += "h+='<div class=\"section\"><h2 data-i18n=\"memory_details\" data-i18n-prefix=\"💾\">'+tr('memory_details')+'</h2>';";
  js += "h+='<h3 data-i18n=\"flash_memory\" data-i18n-prefix=\"📦\">'+tr('flash_memory')+'</h3><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"real_size\">'+tr('real_size')+'</div><div class=\"info-value\">'+(d.memory.flash.real/1048576).toFixed(2)+' MB</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"flash_type\">'+tr('flash_type')+'</div><div class=\"info-value\">'+d.memory.flash.type+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"flash_speed\">'+tr('flash_speed')+'</div><div class=\"info-value\">'+d.memory.flash.speed+' MHz</div></div>';";
  js += "h+='</div>';";

  js += "h+='<h3 data-i18n=\"internal_sram\" data-i18n-prefix=\"🧠\">'+tr('internal_sram')+'</h3><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"total_size\">'+tr('total_size')+'</div><div class=\"info-value\" id=\"sram-total\">'+(d.memory.sram.total/1024).toFixed(2)+' KB</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"free\">'+tr('free')+'</div><div class=\"info-value\" id=\"sram-free\">'+(d.memory.sram.free/1024).toFixed(2)+' KB</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"used\">'+tr('used')+'</div><div class=\"info-value\" id=\"sram-used\">'+(d.memory.sram.used/1024).toFixed(2)+' KB</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"memory_fragmentation\">'+tr('memory_fragmentation')+'</div><div class=\"info-value\" id=\"fragmentation\">'+d.memory.fragmentation.toFixed(1)+'%</div></div>';";
  js += "h+='</div>';";
  js += "const sramPct=((d.memory.sram.used/d.memory.sram.total)*100).toFixed(1);";
  js += "h+='<div class=\"progress-bar\"><div class=\"progress-fill\" id=\"sram-progress\" style=\"width:'+sramPct+'%\">'+sramPct+'%</div></div>';";

  js += "if(d.memory.psram.total>0){";
  js += "h+='<h3 data-i18n=\"psram_external\" data-i18n-prefix=\"📦\">'+tr('psram_external')+'</h3><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"total_size\">'+tr('total_size')+'</div><div class=\"info-value\" id=\"psram-total\">'+(d.memory.psram.total/1048576).toFixed(2)+' MB</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"free\">'+tr('free')+'</div><div class=\"info-value\" id=\"psram-free\">'+(d.memory.psram.free/1048576).toFixed(2)+' MB</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"used\">'+tr('used')+'</div><div class=\"info-value\" id=\"psram-used\">'+(d.memory.psram.used/1048576).toFixed(2)+' MB</div></div>';";
  js += "h+='</div>';";
  js += "const psramPct=((d.memory.psram.used/d.memory.psram.total)*100).toFixed(1);";
  js += "h+='<div class=\"progress-bar\"><div class=\"progress-fill\" id=\"psram-progress\" style=\"width:'+psramPct+'%\">'+psramPct+'%</div></div>';";
  js += "}";
  js += "h+='</div>';";

  js += "h+='<div class=\"section\"><h2 data-i18n=\"wifi_connection\" data-i18n-prefix=\"📡\">'+tr('wifi_connection')+'</h2><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"connected_ssid\">'+tr('connected_ssid')+'</div><div class=\"info-value\">'+(d.wifi.ssid||'')+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"signal_power\">'+tr('signal_power')+'</div><div class=\"info-value\">'+d.wifi.rssi+' dBm</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"signal_quality\">'+tr('signal_quality')+'</div><div class=\"info-value\">'+(d.wifi.quality_key?tr(d.wifi.quality_key):d.wifi.quality)+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"ip_address\">'+tr('ip_address')+'</div><div class=\"info-value\">'+(d.wifi.ip||'')+'</div></div>';";
  js += "h+='</div></div>';";

  js += "if(d.bluetooth){";
  js += "const bt=d.bluetooth;";
  js += "const supportKey=bt.support_key||(bt.supported?'bluetooth_support_yes':'bluetooth_support_no');";
  js += "const statusText=bt.status_key?tr(bt.status_key):(bt.status||'');";
  js += "const advertisingText=bt.advertising_key?tr(bt.advertising_key):(bt.advertising_label||'');";
  js += "const connectionText=bt.connection_key?tr(bt.connection_key):(bt.connection_label||'');";
  js += "h+='<div class=\"section\"><h2 data-i18n=\"bluetooth_section\" data-i18n-prefix=\"🅱️\">'+tr('bluetooth_section')+'</h2><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"bluetooth_support_label\">'+tr('bluetooth_support_label')+'</div><div class=\"info-value\" id=\"overview-bt-support\">'+tr(supportKey)+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"bluetooth_status\">'+tr('bluetooth_status')+'</div><div class=\"info-value\" id=\"overview-bt-status\">'+statusText+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"bluetooth_name\">'+tr('bluetooth_name')+'</div><div class=\"info-value\" id=\"overview-bt-name\">'+(bt.name||'')+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"bluetooth_mac\">'+tr('bluetooth_mac')+'</div><div class=\"info-value\" id=\"overview-bt-mac\">'+(bt.mac||'')+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"bluetooth_advertising_label\">'+tr('bluetooth_advertising_label')+'</div><div class=\"info-value\" id=\"overview-bt-advertising\">'+advertisingText+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"bluetooth_connection_label\">'+tr('bluetooth_connection_label')+'</div><div class=\"info-value\" id=\"overview-bt-connection\">'+connectionText+'</div></div>';";
  js += "h+='</div></div>';";
  js += "}";

  js += "h+='<div class=\"section\"><h2 data-i18n=\"gpio_interfaces\" data-i18n-prefix=\"🔌\">'+tr('gpio_interfaces')+'</h2><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"total_gpio\">'+tr('total_gpio')+'</div><div class=\"info-value\">'+d.gpio.total+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"i2c_peripherals\">'+tr('i2c_peripherals')+'</div><div class=\"info-value\">'+d.gpio.i2c_count+'</div></div>';";
  js += "h+='<div class=\"info-item\" style=\"grid-column:1/-1\"><div class=\"info-label\" data-i18n=\"detected_addresses\">'+tr('detected_addresses')+'</div><div class=\"info-value\">'+(d.gpio.i2c_devices||'')+'</div></div>';";
  js += "h+='</div></div>';";
  js += "return h;";
  js += "}";

  // Build LEDs - localisation complète
  js += "function buildLeds(d){";
  js += "let h='<div class=\"section\"><h2 data-i18n=\"builtin_led\" data-i18n-prefix=\"💡\">'+tr('builtin_led')+'</h2><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"gpio\">'+tr('gpio')+'</div><div class=\"info-value\"><span data-i18n=\"gpio\">'+tr('gpio')+'</span> '+d.builtin.pin+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"status\">'+tr('status')+'</div><div class=\"info-value\" id=\"builtin-led-status\">'+(d.builtin.status||'')+'</div></div>';";
  js += "h+='<div class=\"info-item\" style=\"grid-column:1/-1;text-align:center\">';";
  js += "h+='<button class=\"btn btn-primary\" data-i18n=\"full_test\" data-i18n-prefix=\"🧪\" onclick=\"testBuiltinLED()\">'+tr('full_test')+'</button> ';";
  js += "h+='<button class=\"btn btn-success\" data-i18n=\"blink\" data-i18n-prefix=\"⚡\" onclick=\"ledBlink()\">'+tr('blink')+'</button> ';";
  js += "h+='<button class=\"btn btn-info\" data-i18n=\"fade\" data-i18n-prefix=\"🌊\" onclick=\"ledFade()\">'+tr('fade')+'</button> ';";
  js += "h+='<button class=\"btn btn-warning\" data-i18n=\"turn_on\" data-i18n-prefix=\"💡\" onclick=\"ledOn()\">'+tr('turn_on')+'</button> ';";
  js += "h+='<button class=\"btn btn-danger\" data-i18n=\"turn_off\" data-i18n-prefix=\"⭕\" onclick=\"ledOff()\">'+tr('turn_off')+'</button>';";
  js += "h+='</div></div></div>';";

  js += "h+='<div class=\"section\"><h2 data-i18n=\"neopixel\" data-i18n-prefix=\"🌈\">'+tr('neopixel')+'</h2><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"gpio\">'+tr('gpio')+'</div><div class=\"info-value\"><span data-i18n=\"gpio\">'+tr('gpio')+'</span> '+d.neopixel.pin+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"led_count\">'+tr('led_count')+'</div><div class=\"info-value\">'+d.neopixel.count+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"status\">'+tr('status')+'</div><div class=\"info-value\" id=\"neopixel-status\">'+(d.neopixel.status||'')+'</div></div>';";
  js += "h+='<div class=\"info-item\" style=\"grid-column:1/-1;text-align:center\">';";
  js += "h+='<button class=\"btn btn-primary\" data-i18n=\"full_test\" data-i18n-prefix=\"🧪\" onclick=\"testNeoPixel()\">'+tr('full_test')+'</button><br><br>';";
  js += "h+='<strong data-i18n=\"animations\" data-i18n-suffix=\" :\">'+tr('animations')+'</strong><br>';";
  js += "h+='<button class=\"btn btn-primary\" data-i18n=\"rainbow\" data-i18n-prefix=\"🌈\" onclick=\"neoPattern(\\'rainbow\\')\">'+tr('rainbow')+'</button> ';";
  js += "h+='<button class=\"btn btn-success\" data-i18n=\"blink\" data-i18n-prefix=\"⚡\" onclick=\"neoPattern(\\'blink\\')\">'+tr('blink')+'</button> ';";
  js += "h+='<button class=\"btn btn-info\" data-i18n=\"fade\" data-i18n-prefix=\"🌊\" onclick=\"neoPattern(\\'fade\\')\">'+tr('fade')+'</button> ';";
  js += "h+='<button class=\"btn btn-warning\" data-i18n=\"chase\" data-i18n-prefix=\"🏃\" onclick=\"neoPattern(\\'chase\\')\">'+tr('chase')+'</button><br><br>';";
  js += "h+='<strong data-i18n=\"custom_color\" data-i18n-suffix=\" :\">'+tr('custom_color')+'</strong><br>';";
  js += "h+='<input type=\"color\" id=\"neoColor\" value=\"#ff0000\" style=\"height:50px;width:120px;border:none;border-radius:5px;cursor:pointer\"> ';";
  js += "h+='<button class=\"btn btn-primary\" data-i18n=\"apply_color\" data-i18n-prefix=\"🎨\" onclick=\"neoCustomColor()\">'+tr('apply_color')+'</button><br><br>';";
  js += "h+='<button class=\"btn btn-danger\" data-i18n=\"turn_off_all\" data-i18n-prefix=\"⭕\" onclick=\"neoPattern(\\'off\\')\">'+tr('turn_off_all')+'</button>';";
  js += "h+='</div></div></div>';";
  js += "return h;";
  js += "}";

  // --- [NEW FEATURE] Traduction complète des interactions OLED ---
  js += "function buildScreens(d){";
  js += "const rotation=(typeof d.oled.rotation!=='undefined')?d.oled.rotation:0;";
  js += "const hasOled=d&&d.oled&&((typeof d.oled.available==='undefined')?true:!!d.oled.available);";
  js += "let h='<div class=\"section\"><h2 data-i18n=\"oled_screen\" data-i18n-prefix=\"🖥️\">'+tr('oled_screen')+'</h2><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"status\">'+tr('status')+'</div><div class=\"info-value\" id=\"oled-status\">'+(d.oled.status||'')+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"i2c_pins\">'+tr('i2c_pins')+'</div><div class=\"info-value\" id=\"oled-pins\">SDA:'+d.oled.pins.sda+' SCL:'+d.oled.pins.scl+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"rotation\">'+tr('rotation')+'</div><div class=\"info-value\" id=\"oled-rotation-display\">'+rotation+'</div></div>';";
  js += "h+='</div>';";
  js += "h+='<div class=\"info-item\" style=\"grid-column:1/-1;text-align:center\">';";
  js += "h+='SDA: <input type=\"number\" id=\"oledSDA\" value=\"'+d.oled.pins.sda+'\" min=\"0\" max=\"48\" style=\"width:70px\"> ';";
  js += "h+='SCL: <input type=\"number\" id=\"oledSCL\" value=\"'+d.oled.pins.scl+'\" min=\"0\" max=\"48\" style=\"width:70px\"> ';";
  js += "h+='<span data-i18n=\"rotation\" data-i18n-suffix=\" :\">'+tr('rotation')+'</span> <select id=\"oledRotation\" style=\"width:90px;padding:10px;border:2px solid #ddd;border-radius:5px\">';";
  js += "for(let i=0;i<4;i++){h+='<option value=\\\''+i+'\\\''+(i===rotation?' selected':'')+'>'+i+'</option>';};";
  js += "h+='</select> ';";
  js += "h+='<button class=\"btn btn-info\" data-i18n=\"apply_redetect\" data-i18n-prefix=\"🔄\" onclick=\"configOLED()\">'+tr('apply_redetect')+'</button>';";
  js += "h+='</div>';";
  js += "if(hasOled){";
  js += "h+='<div style=\"margin-top:15px\"><button class=\"btn btn-primary\" data-i18n=\"full_test\" data-i18n-prefix=\"🧪\" data-i18n-suffix=\" (25s)\" onclick=\"testOLED()\">'+tr('full_test')+'</button></div>';";
  js += "h+='<div class=\"oled-step-grid\" style=\"margin-top:15px;display:grid;grid-template-columns:repeat(auto-fit,minmax(180px,1fr));gap:10px\">';";
  js += "h+='<button class=\"btn btn-secondary\" data-i18n=\"oled_step_welcome\" data-i18n-prefix=\"🏁\" onclick=\"oledStep(\\\'welcome\\\')\">'+tr('oled_step_welcome')+'</button>';";
  js += "h+='<button class=\"btn btn-secondary\" data-i18n=\"oled_step_big_text\" data-i18n-prefix=\"🔠\" onclick=\"oledStep(\\\'big_text\\\')\">'+tr('oled_step_big_text')+'</button>';";
  js += "h+='<button class=\"btn btn-secondary\" data-i18n=\"oled_step_text_sizes\" data-i18n-prefix=\"🔤\" onclick=\"oledStep(\\\'text_sizes\\\')\">'+tr('oled_step_text_sizes')+'</button>';";
  js += "h+='<button class=\"btn btn-secondary\" data-i18n=\"oled_step_shapes\" data-i18n-prefix=\"🟦\" onclick=\"oledStep(\\\'shapes\\\')\">'+tr('oled_step_shapes')+'</button>';";
  js += "h+='<button class=\"btn btn-secondary\" data-i18n=\"oled_step_horizontal_lines\" data-i18n-prefix=\"📏\" onclick=\"oledStep(\\\'horizontal_lines\\\')\">'+tr('oled_step_horizontal_lines')+'</button>';";
  js += "h+='<button class=\"btn btn-secondary\" data-i18n=\"oled_step_diagonals\" data-i18n-prefix=\"📐\" onclick=\"oledStep(\\\'diagonals\\\')\">'+tr('oled_step_diagonals')+'</button>';";
  js += "h+='<button class=\"btn btn-secondary\" data-i18n=\"oled_step_moving_square\" data-i18n-prefix=\"[SQ]\" onclick=\"oledStep(\\\'moving_square\\\')\">'+tr('oled_step_moving_square')+'</button>';";
  js += "h+='<button class=\"btn btn-secondary\" data-i18n=\"oled_step_progress_bar\" data-i18n-prefix=\"📊\" onclick=\"oledStep(\\\'progress_bar\\\')\">'+tr('oled_step_progress_bar')+'</button>';";
  js += "h+='<button class=\"btn btn-secondary\" data-i18n=\"oled_step_scroll_text\" data-i18n-prefix=\"📜\" onclick=\"oledStep(\\\'scroll_text\\\')\">'+tr('oled_step_scroll_text')+'</button>';";
  js += "h+='<button class=\"btn btn-secondary\" data-i18n=\"oled_step_final_message\" data-i18n-prefix=\"[OK]\" onclick=\"oledStep(\\\'final_message\\\')\">'+tr('oled_step_final_message')+'</button>';";
  js += "h+='</div>';";
  js += "h+='<div style=\"margin-top:15px\">';";
  js += "h+='<label for=\"oledText\" style=\"display:block;margin-bottom:8px;font-weight:bold;color:#667eea\" data-i18n=\"custom_message\">'+tr('custom_message')+'</label>';";
  js += "h+='<textarea id=\"oledText\" rows=\"3\" style=\"width:100%;padding:10px;border:2px solid #ddd;border-radius:8px\" data-i18n-placeholder=\"custom_message\" placeholder=\"'+tr('custom_message')+'\"></textarea>';";
  js += "h+='<div style=\"margin-top:10px\"><button class=\"btn btn-success\" data-i18n=\"show_message\" data-i18n-prefix=\"📤\" onclick=\"oledDisplayText()\">'+tr('show_message')+'</button></div>';";
  js += "h+='<p style=\"margin-top:12px;color:#555\" data-i18n=\"changes_pins\">'+tr('changes_pins')+'</p>';";
  js += "}else{";
  js += "h+='<p class=\"status-live error\" data-i18n=\"no_detected\">'+tr('no_detected')+'</p>';";
  js += "h+='<p style=\"margin-top:10px;color:#555\" data-i18n=\"check_wiring\">'+tr('check_wiring')+'</p>';";
  js += "}";
  js += "h+='</div></div>';";
  js += "return h;";
  js += "}";

  // Build Tests - localisation complète
  js += "function buildTests(){";
  js += "let h='';";
  js += "h+='<div class=\"section\"><h2 data-i18n=\"adc_test\" data-i18n-prefix=\"📊\">'+tr('adc_test')+'</h2>';";
  js += "h+='<p data-i18n=\"adc_desc\">'+tr('adc_desc')+'</p>';";
  js += "h+='<div style=\"text-align:center;margin:20px 0\"><button class=\"btn btn-primary\" data-i18n=\"start_adc_test\" data-i18n-prefix=\"▶️\" onclick=\"testADC()\">'+tr('start_adc_test')+'</button></div>';";
  js += "h+='<div id=\"adc-status\" class=\"status-live\" data-i18n=\"click_to_test\">'+tr('click_to_test')+'</div>';";
  js += "h+='<div id=\"adc-results\" class=\"info-grid\"></div></div>';";
  js += "h+='<div class=\"section\"><h2 data-i18n=\"memory_stress\" data-i18n-prefix=\"🔥\">'+tr('memory_stress')+'</h2>';";
  js += "h+='<p data-i18n=\"stress_desc\">'+tr('stress_desc')+'</p>';";
  js += "h+='<div style=\"text-align:center;margin:20px 0\"><button class=\"btn btn-danger\" data-i18n=\"start_stress\" data-i18n-prefix=\"🚀\" onclick=\"runStressTest()\">'+tr('start_stress')+'</button></div>';";
  js += "h+='<p style=\"color:#dc3545;font-weight:bold;text-align:center\" data-i18n=\"stress_warning\" data-i18n-prefix=\"⚠️\">'+tr('stress_warning')+'</p>';";
  js += "h+='<div id=\"stress-status\" class=\"status-live\" data-i18n=\"not_tested\">'+tr('not_tested')+'</div>';";
  js += "h+='<div id=\"stress-results\" class=\"info-grid\"></div></div>';";
  js += "return h;";
  js += "}";

  // Build GPIO - localisation complète
  js += "function buildGpio(){";
  js += "let h='<div class=\"section\"><h2 data-i18n=\"gpio_test\" data-i18n-prefix=\"🔌\">'+tr('gpio_test')+'</h2>';";
  js += "h+='<p data-i18n=\"gpio_desc\">'+tr('gpio_desc')+'</p>';";
  js += "h+='<div style=\"text-align:center;margin:20px 0\"><button class=\"btn btn-primary\" data-i18n=\"test_all_gpio\" data-i18n-prefix=\"🧪\" onclick=\"testAllGPIO()\">'+tr('test_all_gpio')+'</button></div>';";
  js += "h+='<div id=\"gpio-status\" class=\"status-live\" data-i18n=\"click_to_test\">'+tr('click_to_test')+'</div>';";
  js += "h+='<p style=\"margin-top:10px;color:#555\" data-i18n=\"gpio_warning\">'+tr('gpio_warning')+'</p>';";
  js += "h+='<div id=\"gpio-results\" class=\"gpio-grid\"></div></div>';";
  js += "return h;";
  js += "}";

  // --- [NEW FEATURE] Onglet Sans fil combiné WiFi/Bluetooth ---
  js += "function buildWireless(){";
  js += "let h='<div class=\"section\"><h2 data-i18n=\"wifi_scanner\" data-i18n-prefix=\"📡\">'+tr('wifi_scanner')+'</h2>';";
  js += "h+='<p data-i18n=\"wifi_desc\">'+tr('wifi_desc')+'</p>';";
  js += "h+='<div style=\"text-align:center;margin:20px 0\"><button class=\"btn btn-primary\" data-i18n=\"scan_networks\" data-i18n-prefix=\"🔍\" onclick=\"scanWiFi()\">'+tr('scan_networks')+'</button></div>';";
  js += "h+='<div id=\"wifi-status\" class=\"status-live\" data-i18n=\"click_to_scan\">'+tr('click_to_scan')+'</div>';";
  js += "h+='<div id=\"wifi-results\" class=\"wifi-list\"></div></div>';";
  js += "h+='<div class=\"section\"><h2 data-i18n=\"bluetooth_section\" data-i18n-prefix=\"🅱️\">'+tr('bluetooth_section')+'</h2>';";
  js += "h+='<div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"bluetooth_support_label\">'+tr('bluetooth_support_label')+'</div><div class=\"info-value\" id=\"bluetooth-support\" data-i18n=\"bluetooth_support_no\">'+tr('bluetooth_support_no')+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"bluetooth_status\">'+tr('bluetooth_status')+'</div><div class=\"info-value\" id=\"bluetooth-status\" data-i18n=\"bluetooth_disabled\">'+tr('bluetooth_disabled')+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"bluetooth_name\">'+tr('bluetooth_name')+'</div><div class=\"info-value\" id=\"bluetooth-name\"></div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"bluetooth_mac\">'+tr('bluetooth_mac')+'</div><div class=\"info-value\" id=\"bluetooth-mac\"></div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"bluetooth_advertising_label\">'+tr('bluetooth_advertising_label')+'</div><div class=\"info-value\" id=\"bluetooth-advertising\" data-i18n=\"bluetooth_not_advertising\">'+tr('bluetooth_not_advertising')+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"bluetooth_connection_label\">'+tr('bluetooth_connection_label')+'</div><div class=\"info-value\" id=\"bluetooth-connection\" data-i18n=\"bluetooth_client_disconnected\">'+tr('bluetooth_client_disconnected')+'</div></div>';";
  js += "h+='</div>';";
  js += "h+='<div class=\"card\" style=\"margin-top:15px\">';";
  js += "h+='<div style=\"text-align:center\">';";
  js += "h+='<input type=\"text\" id=\"bluetoothNameInput\" maxlength=\"29\" style=\"width:260px;padding:10px;border:2px solid #ddd;border-radius:8px\" data-i18n-placeholder=\"bluetooth_placeholder\" placeholder=\"'+tr('bluetooth_placeholder')+'\">';";
  js += "h+='<div style=\"margin-top:10px\">';";
  js += "h+='<button id=\"bluetooth-enable-btn\" class=\"btn btn-success\" data-i18n=\"bluetooth_enable\" data-i18n-prefix=\"▶️\" onclick=\"setBluetoothState(true)\">'+tr('bluetooth_enable')+'</button> ';";
  js += "h+='<button id=\"bluetooth-disable-btn\" class=\"btn btn-danger\" data-i18n=\"bluetooth_disable\" data-i18n-prefix=\"⏹️\" onclick=\"setBluetoothState(false)\">'+tr('bluetooth_disable')+'</button>';";
  js += "h+='</div>';";
  js += "h+='<div style=\"margin-top:10px\">';";
  js += "h+='<button class=\"btn btn-primary\" data-i18n=\"bluetooth_rename\" data-i18n-prefix=\"✏️\" onclick=\"renameBluetooth()\">'+tr('bluetooth_rename')+'</button> ';";
  js += "h+='<button class=\"btn btn-warning\" data-i18n=\"bluetooth_reset\" data-i18n-prefix=\"♻️\" onclick=\"resetBluetoothName()\">'+tr('bluetooth_reset')+'</button>';";
  js += "h+='</div>';";
  js += "h+='</div>';";
  js += "h+='<div id=\"bluetooth-action-status\" class=\"status-live\" data-i18n=\"bluetooth_status\">'+tr('bluetooth_status')+'</div>';";
  js += "h+='</div>';";
  js += "h+='<div class=\"card\" style=\"margin-top:20px\">';";
  js += "h+='<div style=\"text-align:center\"><button class=\"btn btn-info\" data-i18n=\"bluetooth_scan\" data-i18n-prefix=\"🔍\" onclick=\"scanBluetoothDevices()\">'+tr('bluetooth_scan')+'</button></div>';";
  js += "h+='<p style=\"margin-top:10px;color:#555\" data-i18n=\"bluetooth_scan_hint\">'+tr('bluetooth_scan_hint')+'</p>';";
  js += "h+='<div id=\"bluetooth-scan-status\" class=\"status-live\" data-i18n=\"click_to_scan\">'+tr('click_to_scan')+'</div>';";
  js += "h+='<div id=\"bluetooth-scan-results\" class=\"info-grid\"></div>';";
  js += "h+='</div></div>';";
  js += "return h;";
  js += "}";

  // Build Benchmark - localisation complète
  js += "function buildBenchmark(){";
  js += "let h='<div class=\"section\"><h2 data-i18n=\"performance_bench\" data-i18n-prefix=\"⚡\">'+tr('performance_bench')+'</h2>';";
  js += "h+='<p data-i18n=\"benchmark_desc\">'+tr('benchmark_desc')+'</p>';";
  js += "h+='<div style=\"text-align:center;margin:20px 0\"><button class=\"btn btn-primary\" data-i18n=\"run_benchmarks\" data-i18n-prefix=\"🚀\" onclick=\"runBenchmarks()\">'+tr('run_benchmarks')+'</button></div>';";
  js += "h+='<div class=\"info-grid\" id=\"benchmark-results\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"cpu_benchmark\">'+tr('cpu_benchmark')+'</div><div class=\"info-value\" id=\"cpu-bench\" data-i18n=\"not_tested\">'+tr('not_tested')+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"memory_benchmark\">'+tr('memory_benchmark')+'</div><div class=\"info-value\" id=\"mem-bench\" data-i18n=\"not_tested\">'+tr('not_tested')+'</div></div>';";
  js += "h+='</div></div>';";
  js += "return h;";
  js += "}";

  // Build Export - localisation complète
  js += "function buildExport(){";
  js += "let h='<div class=\"section\"><h2 data-i18n=\"data_export\" data-i18n-prefix=\"💾\">'+tr('data_export')+'</h2>';";
  js += "h+='<div style=\"display:grid;grid-template-columns:repeat(auto-fit,minmax(250px,1fr));gap:20px;margin-top:20px\">';";
  js += "h+='<div class=\"card\" style=\"text-align:center;padding:30px\"><h3 style=\"color:#667eea\" data-i18n=\"txt_file\">'+tr('txt_file')+'</h3><p style=\"font-size:0.9em;color:#666;margin:15px 0\" data-i18n=\"readable_report\">'+tr('readable_report')+'</p><a href=\"/export/txt\" class=\"btn btn-primary\" data-i18n=\"download_txt\" data-i18n-prefix=\"📥\">'+tr('download_txt')+'</a></div>';";
  js += "h+='<div class=\"card\" style=\"text-align:center;padding:30px\"><h3 style=\"color:#3a7bd5\" data-i18n=\"json_file\">'+tr('json_file')+'</h3><p style=\"font-size:0.9em;color:#666;margin:15px 0\" data-i18n=\"structured_format\">'+tr('structured_format')+'</p><a href=\"/export/json\" class=\"btn btn-info\" data-i18n=\"download_json\" data-i18n-prefix=\"📥\">'+tr('download_json')+'</a></div>';";
  js += "h+='<div class=\"card\" style=\"text-align:center;padding:30px\"><h3 style=\"color:#56ab2f\" data-i18n=\"csv_file\">'+tr('csv_file')+'</h3><p style=\"font-size:0.9em;color:#666;margin:15px 0\" data-i18n=\"for_excel\">'+tr('for_excel')+'</p><a href=\"/export/csv\" class=\"btn btn-success\" data-i18n=\"download_csv\" data-i18n-prefix=\"📥\">'+tr('download_csv')+'</a></div>';";
  js += "h+='<div class=\"card\" style=\"text-align:center;padding:30px\"><h3 style=\"color:#667eea\" data-i18n=\"printable_version\">'+tr('printable_version')+'</h3><p style=\"font-size:0.9em;color:#666;margin:15px 0\" data-i18n=\"pdf_format\">'+tr('pdf_format')+'</p><a href=\"/print\" target=\"_blank\" class=\"btn btn-primary\" data-i18n=\"open\" data-i18n-prefix=\"🖨️\">'+tr('open')+'</a></div>';";
  js += "h+='</div></div>';";
  js += "return h;";
  js += "}";

  // --- [NEW FEATURE] Build Sensors - localisation complète ---
  // --- [REORGANIZATION] Build Display & Signal - Combine LEDs, Screens, RGB, Buzzer ---
  js += "function buildDisplaySignal(ledsData,screensData){";
  js += "let h=buildLeds(ledsData);";
  js += "h+=buildScreens(screensData);";

  // LED RGB Section
  js += "h+='<div class=\"section\"><h2 data-i18n=\"rgb_led\" data-i18n-prefix=\"💡\">'+tr('rgb_led')+'</h2>';";
  js += "h+='<p data-i18n=\"rgb_led_desc\">'+tr('rgb_led_desc')+'</p>';";
  js += "h+='<div class=\"card\"><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"rgb_led_pins\">'+tr('rgb_led_pins')+'</div>';";
  js += "h+='<div style=\"display:flex;gap:5px\">';";
  js += "h+='<input type=\"number\" id=\"rgbPinR\" value=\"14\" style=\"width:60px\" placeholder=\"R\"/>';";
  js += "h+='<input type=\"number\" id=\"rgbPinG\" value=\"13\" style=\"width:60px\" placeholder=\"G\"/>';";
  js += "h+='<input type=\"number\" id=\"rgbPinB\" value=\"12\" style=\"width:60px\" placeholder=\"B\"/>';";
  js += "h+='<button class=\"btn btn-info\" onclick=\"applyRGBConfig()\" data-i18n=\"apply_config\">'+tr('apply_config')+'</button></div></div></div>';";
  js += "h+='<div style=\"text-align:center;margin:15px 0\">';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"testRGBLed()\" data-i18n=\"test_rgb_led\" data-i18n-prefix=\"▶️\">'+tr('test_rgb_led')+'</button> ';";
  js += "h+='<button class=\"btn btn-danger\" onclick=\"setRGBColor(255,0,0)\" data-i18n=\"red\">'+tr('red')+'</button> ';";
  js += "h+='<button class=\"btn btn-success\" onclick=\"setRGBColor(0,255,0)\" data-i18n=\"green\">'+tr('green')+'</button> ';";
  js += "h+='<button class=\"btn btn-info\" onclick=\"setRGBColor(0,0,255)\" data-i18n=\"blue\">'+tr('blue')+'</button> ';";
  js += "h+='<button class=\"btn\" style=\"background:#fff;color:#000;border:1px solid #ddd\" onclick=\"setRGBColor(255,255,255)\" data-i18n=\"white\">'+tr('white')+'</button> ';";
  js += "h+='<button class=\"btn\" style=\"background:#333\" onclick=\"setRGBColor(0,0,0)\" data-i18n=\"off\">'+tr('off')+'</button>';";
  js += "h+='</div><div id=\"rgb-status\" class=\"status-live\" data-i18n=\"click_to_test\">'+tr('click_to_test')+'</div></div>';";

  // Buzzer Section
  js += "h+='<div class=\"section\"><h2 data-i18n=\"buzzer\" data-i18n-prefix=\"🔔\">'+tr('buzzer')+'</h2>';";
  js += "h+='<p data-i18n=\"buzzer_desc\">'+tr('buzzer_desc')+'</p>';";
  js += "h+='<div class=\"card\"><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"buzzer_pin\">'+tr('buzzer_pin')+'</div>';";
  js += "h+='<div style=\"display:flex;gap:5px\">';";
  js += "h+='<input type=\"number\" id=\"buzzerPin\" value=\"16\" style=\"width:80px\"/>';";
  js += "h+='<button class=\"btn btn-info\" onclick=\"applyBuzzerConfig()\" data-i18n=\"apply_config\">'+tr('apply_config')+'</button></div></div></div>';";
  js += "h+='<div style=\"text-align:center;margin:15px 0\">';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"testBuzzer()\" data-i18n=\"test_buzzer\" data-i18n-prefix=\"▶️\">'+tr('test_buzzer')+'</button> ';";
  js += "h+='<button class=\"btn btn-warning\" onclick=\"playTone(1000,300)\" data-i18n=\"beep\">'+tr('beep')+'</button>';";
  js += "h+='</div><div id=\"buzzer-status\" class=\"status-live\" data-i18n=\"click_to_test\">'+tr('click_to_test')+'</div></div>';";

  js += "return h;";
  js += "}";

  // --- [REORGANIZATION] Build Hardware Tests - Combine GPIO, ADC, PWM, Stress ---
  js += "function buildHardwareTests(){";
  js += "let h=buildGpio();";
  js += "h+=buildTests();";
  js += "return h;";
  js += "}";

  // --- [REORGANIZATION] Build Sensors - Environmental sensors only (DHT11, Light, Distance, Motion) ---
  js += "function buildSensors(){";
  js += "let h='<div class=\"section\"><h2 data-i18n=\"sensors_section\" data-i18n-prefix=\"📡\">'+tr('sensors_section')+'</h2>';";

  // DHT11 Section
  js += "h+='<h3 data-i18n=\"dht11_sensor\" data-i18n-prefix=\"🌡️\">'+tr('dht11_sensor')+'</h3>';";
  js += "h+='<p data-i18n=\"dht11_desc\">'+tr('dht11_desc')+'</p>';";
  js += "h+='<div class=\"card\"><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"dht11_pin\">'+tr('dht11_pin')+'</div>';";
  js += "h+='<div style=\"display:flex;gap:5px\">';";
  js += "h+='<input type=\"number\" id=\"dht11Pin\" value=\"4\" style=\"width:80px\"/>';";
  js += "h+='<button class=\"btn btn-info\" onclick=\"applyDHT11Config()\" data-i18n=\"apply_config\">'+tr('apply_config')+'</button></div></div></div>';";
  js += "h+='<div style=\"text-align:center;margin:15px 0\">';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"testDHT11()\" data-i18n=\"test_dht11\" data-i18n-prefix=\"▶️\">'+tr('test_dht11')+'</button>';";
  js += "h+='</div><div id=\"dht11-status\" class=\"status-live\" data-i18n=\"click_to_test\">'+tr('click_to_test')+'</div>';";
  js += "h+='<div id=\"dht11-results\" class=\"info-grid\"></div></div>';";

  // Light Sensor Section
  js += "h+='<h3 data-i18n=\"light_sensor\" data-i18n-prefix=\"☀️\">'+tr('light_sensor')+'</h3>';";
  js += "h+='<p data-i18n=\"light_sensor_desc\">'+tr('light_sensor_desc')+'</p>';";
  js += "h+='<div class=\"card\"><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"light_sensor_pin\">'+tr('light_sensor_pin')+'</div>';";
  js += "h+='<div style=\"display:flex;gap:5px\">';";
  js += "h+='<input type=\"number\" id=\"lightPin\" value=\"17\" style=\"width:80px\"/>';";
  js += "h+='<button class=\"btn btn-info\" onclick=\"applyLightConfig()\" data-i18n=\"apply_config\">'+tr('apply_config')+'</button></div></div></div>';";
  js += "h+='<div style=\"text-align:center;margin:15px 0\">';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"testLightSensor()\" data-i18n=\"test_light_sensor\" data-i18n-prefix=\"▶️\">'+tr('test_light_sensor')+'</button>';";
  js += "h+='</div><div id=\"light-status\" class=\"status-live\" data-i18n=\"click_to_test\">'+tr('click_to_test')+'</div>';";
  js += "h+='<div id=\"light-results\" class=\"info-grid\"></div></div>';";

  // Distance Sensor Section
  js += "h+='<h3 data-i18n=\"distance_sensor\" data-i18n-prefix=\"📏\">'+tr('distance_sensor')+'</h3>';";
  js += "h+='<p data-i18n=\"distance_sensor_desc\">'+tr('distance_sensor_desc')+'</p>';";
  js += "h+='<div class=\"card\"><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"distance_pins\">'+tr('distance_pins')+'</div>';";
  js += "h+='<div style=\"display:flex;gap:5px\">';";
  js += "h+='<input type=\"number\" id=\"distTrig\" value=\"7\" style=\"width:60px\" placeholder=\"Trig\"/>';";
  js += "h+='<input type=\"number\" id=\"distEcho\" value=\"8\" style=\"width:60px\" placeholder=\"Echo\"/>';";
  js += "h+='<button class=\"btn btn-info\" onclick=\"applyDistanceConfig()\" data-i18n=\"apply_config\">'+tr('apply_config')+'</button></div></div></div>';";
  js += "h+='<div style=\"text-align:center;margin:15px 0\">';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"testDistanceSensor()\" data-i18n=\"test_distance_sensor\" data-i18n-prefix=\"▶️\">'+tr('test_distance_sensor')+'</button>';";
  js += "h+='</div><div id=\"distance-status\" class=\"status-live\" data-i18n=\"click_to_test\">'+tr('click_to_test')+'</div>';";
  js += "h+='<div id=\"distance-results\" class=\"info-grid\"></div></div>';";

  // Motion Sensor Section
  js += "h+='<h3 data-i18n=\"motion_sensor\" data-i18n-prefix=\"👁️\">'+tr('motion_sensor')+'</h3>';";
  js += "h+='<p data-i18n=\"motion_sensor_desc\">'+tr('motion_sensor_desc')+'</p>';";
  js += "h+='<div class=\"card\"><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"motion_sensor_pin\">'+tr('motion_sensor_pin')+'</div>';";
  js += "h+='<div style=\"display:flex;gap:5px\">';";
  js += "h+='<input type=\"number\" id=\"motionPin\" value=\"6\" style=\"width:80px\"/>';";
  js += "h+='<button class=\"btn btn-info\" onclick=\"applyMotionConfig()\" data-i18n=\"apply_config\">'+tr('apply_config')+'</button></div></div></div>';";
  js += "h+='<div style=\"text-align:center;margin:15px 0\">';";
  js += "h+='<button class=\"btn btn-primary\" onclick=\"testMotionSensor()\" data-i18n=\"test_motion_sensor\" data-i18n-prefix=\"▶️\">'+tr('test_motion_sensor')+'</button>';";
  js += "h+='</div><div id=\"motion-status\" class=\"status-live\" data-i18n=\"click_to_test\">'+tr('click_to_test')+'</div>';";
  js += "h+='<div id=\"motion-results\" class=\"info-grid\"></div></div>';";

  js += "h+='</div>';";
  js += "return h;";
  js += "}";

  // FONCTIONS API - LED
  js += "async function testBuiltinLED(){";
  js += "setStatus('builtin-led-status',{key:'test_in_progress'},null);";
  js += "const r=await fetch('/api/builtin-led-test');const d=await r.json();";
  js += "setStatus('builtin-led-status',d.result,d.success?'success':'error');";
  js += "}";
  js += "async function ledBlink(){";
  js += "setStatus('builtin-led-status',{key:'transmission'},null);";
  js += "const r=await fetch('/api/builtin-led-control?action=blink');const d=await r.json();";
  js += "setStatus('builtin-led-status',d.message,null);";
  js += "}";
  js += "async function ledFade(){";
  js += "setStatus('builtin-led-status',{key:'transmission'},null);";
  js += "const r=await fetch('/api/builtin-led-control?action=fade');const d=await r.json();";
  js += "setStatus('builtin-led-status',d.message,null);";
  js += "}";
  js += "async function ledOff(){";
  js += "setStatus('builtin-led-status',{key:'transmission'},null);";
  js += "const r=await fetch('/api/builtin-led-control?action=off');const d=await r.json();";
  js += "setStatus('builtin-led-status',d.message,null);";
  js += "}";
  js += "async function ledOn(){";
  js += "setStatus('builtin-led-status',{key:'transmission'},null);";
  js += "const r=await fetch('/api/builtin-led-control?action=on');const d=await r.json();";
  js += "setStatus('builtin-led-status',d.message,null);";
  js += "}"; 

  // NeoPixel couleur personnalisée
  js += "async function neoCustomColor(){";
  js += "const color=document.getElementById('neoColor').value;";
  js += "const r=parseInt(color.substr(1,2),16);";
  js += "const g=parseInt(color.substr(3,2),16);";
  js += "const b=parseInt(color.substr(5,2),16);";
  js += "setStatus('neopixel-status',{key:'transmission',suffix:' RGB('+r+','+g+','+b+')'},null);";
  js += "const resp=await fetch('/api/neopixel-color?r='+r+'&g='+g+'&b='+b);";
  js += "const d=await resp.json();";
  js += "setStatus('neopixel-status',d.message,null);";
  js += "}";
  // FONCTIONS API - NeoPixel
  js += "async function testNeoPixel(){";
  js += "setStatus('neopixel-status',{key:'test_in_progress'},null);";
  js += "const r=await fetch('/api/neopixel-test');const d=await r.json();";
  js += "setStatus('neopixel-status',d.result,null);";
  js += "}";
  js += "async function neoPattern(p){";
  js += "setStatus('neopixel-status',{key:'transmission'},null);";
  js += "const r=await fetch('/api/neopixel-pattern?pattern='+p);const d=await r.json();";
  js += "setStatus('neopixel-status',d.message,null);";
  js += "}"; 

  // FONCTIONS API - OLED
  // --- [NEW FEATURE] Localisation complète des statuts OLED ---
  js += "async function testOLED(){";
  js += "setStatus('oled-status',{key:'oled_test_running'},null);";
  js += "const r=await fetch('/api/oled-test');const d=await r.json();";
  js += "setStatus('oled-status',d.result,null);";
  js += "}";
  js += "async function oledStep(step){";
  js += "setStatus('oled-status',{key:'oled_step_running'},null);";
  js += "try{const r=await fetch('/api/oled-step?step='+step);const d=await r.json();setStatus('oled-status',d.message,d.success?'success':'error');}catch(e){setStatus('oled-status',{key:'error_label',suffix:': '+String(e)},'error');}";
  js += "}";

  // FONCTIONS API - Affichage texte OLED
  js += "async function oledDisplayText(){";
  js += "const text=document.getElementById('oledText').value;";
  js += "if(!text){setStatus('oled-status',{key:'oled_message_required'},'error');return;}";
  js += "setStatus('oled-status',{key:'oled_displaying_message'},null);";
  js += "try{const r=await fetch('/api/oled-message?message='+encodeURIComponent(text));const d=await r.json();setStatus('oled-status',d.message,d.success?'success':'error');}catch(e){setStatus('oled-status',{key:'error_label',suffix:': '+String(e)},'error');}";
  js += "}";
  js += "async function configOLED(){";
  js += "setStatus('oled-status',{key:'reconfiguring'},null);";
  js += "const sda=document.getElementById('oledSDA').value;";
  js += "const scl=document.getElementById('oledSCL').value;";
  js += "const rotation=document.getElementById('oledRotation').value;";
  js += "try{";
  js += "const r=await fetch('/api/oled-config?sda='+sda+'&scl='+scl+'&rotation='+rotation);";
  js += "const d=await r.json();";
  js += "const statusPayload=d.message?{text:d.message}:{key:'configuration_invalid'};";
  js += "setStatus('oled-status',statusPayload,d.success?'success':'error');";
  js += "if(d.success){if(typeof d.sda!=='undefined'){document.getElementById('oledSDA').value=d.sda;document.getElementById('oledSCL').value=d.scl;}";
  js += "const pins=document.getElementById('oled-pins');if(pins){pins.textContent='SDA:'+d.sda+' SCL:'+d.scl;}const rotDisplay=document.getElementById('oled-rotation-display');if(rotDisplay){rotDisplay.textContent=d.rotation;}if(document.getElementById('oledRotation')){document.getElementById('oledRotation').value=d.rotation;}}";
  js += "}catch(e){setStatus('oled-status',{key:'error_label',suffix:': '+String(e)},'error');}";
  js += "}";

  // FONCTIONS API - Tests
  // --- [BUGFIX] Localisation dynamique des tests avancés ---
  js += "async function testADC(){";
  js += "setStatus('adc-status',{key:'test_in_progress'},null);";
  js += "const r=await fetch('/api/adc-test');const d=await r.json();";
  js += "let h='';d.readings.forEach(rd=>{h+='<div class=\"info-item\"><div class=\"info-label\">GPIO '+rd.pin+'</div><div class=\"info-value\">'+rd.raw+' ('+rd.voltage.toFixed(2)+'V)</div></div>';});";
  js += "document.getElementById('adc-results').innerHTML=h;";
  js += "setStatus('adc-status',d.result,null);";
  js += "}";

  js += "async function testAllGPIO(){";
  js += "setStatus('gpio-status',{key:'test_in_progress'},null);";
  js += "const r=await fetch('/api/test-gpio');const d=await r.json();";
  js += "let h='';d.results.forEach(g=>{h+='<div class=\"gpio-item '+(g.working?'gpio-ok':'gpio-fail')+'\">GPIO '+g.pin+'<br>'+(g.working?'✅ OK':'❌ FAIL')+'</div>';});";
  js += "document.getElementById('gpio-results').innerHTML=h;";
  js += "setStatus('gpio-status',{key:'gpio_test_complete',replacements:{count:d.results.length}},null);";
  js += "}";

  js += "async function scanWiFi(){";
  js += "setStatus('wifi-status',{key:'wifi_scan_in_progress'},null);";
  js += "const r=await fetch('/api/wifi-scan');const d=await r.json();";
  js += "let h='';d.networks.forEach(n=>{";
  js += "const icon=n.rssi>=-60?'🟢':n.rssi>=-70?'🟡':'🔴';";
  js += "const color=n.rssi>=-60?'#28a745':n.rssi>=-70?'#ffc107':'#dc3545';";
  js += "h+='<div class=\"wifi-item\"><div style=\"display:flex;justify-content:space-between\"><div><strong>'+icon+' '+n.ssid+'</strong><br><small>'+n.bssid+' | '+tr('wifi_channel')+' '+n.channel+'</small></div>';";
  js += "h+='<div style=\"font-size:1.3em;font-weight:bold;color:'+color+'\">'+n.rssi+' dBm</div></div></div>';";
  js += "});";
  js += "document.getElementById('wifi-results').innerHTML=h;";
  js += "setStatus('wifi-status',{key:'wifi_networks_found',replacements:{count:d.networks.length}},null);";
  js += "}";

  // --- [NEW FEATURE] Fonctions Bluetooth Sans fil ---
  js += "function applyBluetoothStatus(d){";
  js += "if(!d){return;}";
  js += "const support=document.getElementById('bluetooth-support');";
  js += "if(support){if(d.support_key){setElementTranslation(support,{key:d.support_key});}else{setElementTranslation(support,{key:d.supported?'bluetooth_support_yes':'bluetooth_support_no'});}}";
  js += "const status=document.getElementById('bluetooth-status');";
  js += "if(status){if(d.status_key){setElementTranslation(status,{key:d.status_key});}else if(typeof d.status==='string'){clearTranslationAttributes(status);status.textContent=d.status;}}";
  js += "const name=document.getElementById('bluetooth-name');";
  js += "if(name){clearTranslationAttributes(name);name.textContent=d.name||'';}";
  js += "const nameInput=document.getElementById('bluetoothNameInput');";
  js += "if(nameInput&&typeof d.name==='string'){nameInput.value=d.name;}";
  js += "const mac=document.getElementById('bluetooth-mac');";
  js += "if(mac){clearTranslationAttributes(mac);mac.textContent=d.mac||'';}";
  js += "const advertising=document.getElementById('bluetooth-advertising');";
  js += "if(advertising){if(d.advertising_key){setElementTranslation(advertising,{key:d.advertising_key});}else if(typeof d.advertising_label==='string'){clearTranslationAttributes(advertising);advertising.textContent=d.advertising_label;}}";
  js += "const connection=document.getElementById('bluetooth-connection');";
  js += "if(connection){if(d.connection_key){setElementTranslation(connection,{key:d.connection_key});}else if(typeof d.connection_label==='string'){clearTranslationAttributes(connection);connection.textContent=d.connection_label;}}";
  js += "const enableBtn=document.getElementById('bluetooth-enable-btn');";
  js += "if(enableBtn){enableBtn.disabled=!d.supported||!!d.enabled;}";
  js += "const disableBtn=document.getElementById('bluetooth-disable-btn');";
  js += "if(disableBtn){disableBtn.disabled=!d.supported||!d.enabled;}";
  js += "}";

  js += "async function refreshBluetoothStatus(){";
  js += "try{const r=await fetch('/api/bluetooth/status');const data=await r.json();applyBluetoothStatus(data);setStatus('bluetooth-action-status',data.message,data.success?'success':'error');}catch(e){setStatus('bluetooth-action-status',{key:'bluetooth_error',suffix:': '+String(e)},'error');}}";

  js += "async function setBluetoothState(enable){";
  js += "setStatus('bluetooth-action-status',{key:enable?'bluetooth_enable':'bluetooth_disable'},null);";
  js += "try{const r=await fetch('/api/bluetooth/toggle?state='+(enable?'on':'off'));const data=await r.json();applyBluetoothStatus(data);setStatus('bluetooth-action-status',data.message,data.success?'success':'error');}catch(e){setStatus('bluetooth-action-status',{key:'bluetooth_error',suffix:': '+String(e)},'error');}}";

  js += "async function renameBluetooth(){";
  js += "const input=document.getElementById('bluetoothNameInput');";
  js += "if(!input){return;}";
  js += "const value=input.value?input.value.trim():'';";
  js += "if(!value){setStatus('bluetooth-action-status',{key:'bluetooth_invalid_name'},'error');return;}";
  js += "setStatus('bluetooth-action-status',{key:'bluetooth_rename'},null);";
  js += "try{const r=await fetch('/api/bluetooth/name?name='+encodeURIComponent(value));const data=await r.json();applyBluetoothStatus(data);setStatus('bluetooth-action-status',data.message,data.success?'success':'error');}catch(e){setStatus('bluetooth-action-status',{key:'bluetooth_error',suffix:': '+String(e)},'error');}}";

  js += "async function resetBluetoothName(){";
  js += "setStatus('bluetooth-action-status',{key:'bluetooth_reset'},null);";
  js += "try{const r=await fetch('/api/bluetooth/reset');const data=await r.json();applyBluetoothStatus(data);setStatus('bluetooth-action-status',data.message,data.success?'success':'error');}catch(e){setStatus('bluetooth-action-status',{key:'bluetooth_error',suffix:': '+String(e)},'error');}}";

  js += "async function scanBluetoothDevices(){";
  js += "setStatus('bluetooth-scan-status',{key:'bluetooth_scan_in_progress'},null);";
  js += "const container=document.getElementById('bluetooth-scan-results');";
  js += "if(container){container.innerHTML='<div class=\"loading\"></div>';}";
  js += "try{const r=await fetch('/api/bluetooth/scan');const data=await r.json();applyBluetoothStatus(data);let h='';if(Array.isArray(data.devices)){data.devices.forEach(dev=>{const rssi=typeof dev.rssi==='number'?dev.rssi:0;const badge=rssi>=-60?'badge-success':(rssi>=-75?'badge-warning':'badge-danger');const label=dev.name||tr('unknown');const address=dev.address||'';h+='<div class=\"info-item\"><div class=\"info-label\">'+label+'</div><div class=\"info-value\"><span class=\"badge '+badge+'\">'+rssi+' dBm</span><br>'+address+'</div></div>';});}if(container){container.innerHTML=h;}setStatus('bluetooth-scan-status',data.message,data.success?'success':'error');}catch(e){if(container){container.innerHTML='';}setStatus('bluetooth-scan-status',{key:'bluetooth_error',suffix:': '+String(e)},'error');}}";

  js += "async function runBenchmarks(){";
  js += "setElementTranslation(document.getElementById('cpu-bench'),{key:'test_in_progress'});";
  js += "setElementTranslation(document.getElementById('mem-bench'),{key:'test_in_progress'});";
  js += "const r=await fetch('/api/benchmark');const d=await r.json();";
  js += "const cpuNode=document.getElementById('cpu-bench');if(cpuNode){clearTranslationAttributes(cpuNode);cpuNode.textContent=d.cpu+' µs';}";
  js += "const memNode=document.getElementById('mem-bench');if(memNode){clearTranslationAttributes(memNode);memNode.textContent=d.memory+' µs';}";
  js += "}";

  js += "async function runStressTest(){";
  js += "setStatus('stress-status',{key:'stress_running'},null);";
  js += "document.getElementById('stress-results').innerHTML='<div class=\"loading\"></div>';";
  js += "try{";
  js += "const r=await fetch('/api/stress-test');";
  js += "const d=await r.json();";
  js += "let h='';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"cpu_time\">'+tr('cpu_time')+'</div><div class=\"info-value\">'+d.cpu_time+' ms</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"memory_time\">'+tr('memory_time')+'</div><div class=\"info-value\">'+d.mem_time+' ms</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"allocations_label\">'+tr('allocations_label')+'</div><div class=\"info-value\">'+d.allocs_success+'/'+d.allocs_total+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"memory_fragmentation\">'+tr('memory_fragmentation')+'</div><div class=\"info-value\">'+d.fragmentation.toFixed(1)+'%</div></div>';";
  js += "document.getElementById('stress-results').innerHTML=h;";
  js += "const successSuffix=d.message?' '+d.message:'';";
  js += "setStatus('stress-status',{key:'completed',prefix:'✅ ',suffix:successSuffix},'success');";
  js += "}catch(e){";
  js += "setStatus('stress-status',{key:'error_label',prefix:'❌ ',suffix:': '+String(e)},'error');";
  js += "document.getElementById('stress-results').innerHTML='';";
  js += "}";
  js += "}";

  // --- [NEW FEATURE] Fonctions API pour les nouveaux capteurs ---
  // RGB LED Functions
  js += "async function applyRGBConfig(){";
  js += "const r=parseInt(document.getElementById('rgbPinR').value);";
  js += "const g=parseInt(document.getElementById('rgbPinG').value);";
  js += "const b=parseInt(document.getElementById('rgbPinB').value);";
  js += "const resp=await fetch('/api/rgb-led-config?r='+r+'&g='+g+'&b='+b);";
  js += "const d=await resp.json();";
  js += "setStatus('rgb-status',d.message,d.success?'success':'error');";
  js += "}";
  js += "async function testRGBLed(){";
  js += "setStatus('rgb-status',{key:'test_in_progress'},null);";
  js += "const r=await fetch('/api/rgb-led-test');";
  js += "const d=await r.json();";
  js += "setStatus('rgb-status',d.result,d.success?'success':'error');";
  js += "}";
  js += "async function setRGBColor(r,g,b){";
  js += "setStatus('rgb-status','RGB('+r+','+g+','+b+')',null);";
  js += "const resp=await fetch('/api/rgb-led-color?r='+r+'&g='+g+'&b='+b);";
  js += "const d=await resp.json();";
  js += "setStatus('rgb-status',d.message,d.success?'success':'error');";
  js += "}";

  // Buzzer Functions
  js += "async function applyBuzzerConfig(){";
  js += "const pin=parseInt(document.getElementById('buzzerPin').value);";
  js += "const resp=await fetch('/api/buzzer-config?pin='+pin);";
  js += "const d=await resp.json();";
  js += "setStatus('buzzer-status',d.message,d.success?'success':'error');";
  js += "}";
  js += "async function testBuzzer(){";
  js += "setStatus('buzzer-status',{key:'test_in_progress'},null);";
  js += "const r=await fetch('/api/buzzer-test');";
  js += "const d=await r.json();";
  js += "setStatus('buzzer-status',d.result,d.success?'success':'error');";
  js += "}";
  js += "async function playTone(freq,duration){";
  js += "setStatus('buzzer-status',freq+' Hz',null);";
  js += "const resp=await fetch('/api/buzzer-tone?freq='+freq+'&duration='+duration);";
  js += "const d=await resp.json();";
  js += "setStatus('buzzer-status',d.message,d.success?'success':'error');";
  js += "}";

  // DHT11 Functions
  js += "async function applyDHT11Config(){";
  js += "const pin=parseInt(document.getElementById('dht11Pin').value);";
  js += "const resp=await fetch('/api/dht11-config?pin='+pin);";
  js += "const d=await resp.json();";
  js += "setStatus('dht11-status',d.message,d.success?'success':'error');";
  js += "}";
  js += "async function testDHT11(){";
  js += "setStatus('dht11-status',{key:'test_in_progress'},null);";
  js += "document.getElementById('dht11-results').innerHTML='';";
  js += "const r=await fetch('/api/dht11-test');";
  js += "const d=await r.json();";
  js += "if(d.success){";
  js += "let h='';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"temperature\">'+tr('temperature')+'</div><div class=\"info-value\">'+d.temperature.toFixed(1)+' °C</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"humidity\">'+tr('humidity')+'</div><div class=\"info-value\">'+d.humidity.toFixed(1)+' %</div></div>';";
  js += "document.getElementById('dht11-results').innerHTML=h;";
  js += "}";
  js += "setStatus('dht11-status',d.result,d.success?'success':'error');";
  js += "}";

  // Light Sensor Functions
  js += "async function applyLightConfig(){";
  js += "const pin=parseInt(document.getElementById('lightPin').value);";
  js += "const resp=await fetch('/api/light-sensor-config?pin='+pin);";
  js += "const d=await resp.json();";
  js += "setStatus('light-status',d.message,d.success?'success':'error');";
  js += "}";
  js += "async function testLightSensor(){";
  js += "setStatus('light-status',{key:'test_in_progress'},null);";
  js += "document.getElementById('light-results').innerHTML='';";
  js += "const r=await fetch('/api/light-sensor-test');";
  js += "const d=await r.json();";
  js += "if(d.success){";
  js += "let h='';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"light_level\">'+tr('light_level')+'</div><div class=\"info-value\">'+d.value+' / 4095</div></div>';";
  js += "document.getElementById('light-results').innerHTML=h;";
  js += "}";
  js += "setStatus('light-status',d.result,d.success?'success':'error');";
  js += "}";

  // Distance Sensor Functions
  js += "async function applyDistanceConfig(){";
  js += "const trig=parseInt(document.getElementById('distTrig').value);";
  js += "const echo=parseInt(document.getElementById('distEcho').value);";
  js += "const resp=await fetch('/api/distance-sensor-config?trig='+trig+'&echo='+echo);";
  js += "const d=await resp.json();";
  js += "setStatus('distance-status',d.message,d.success?'success':'error');";
  js += "}";
  js += "async function testDistanceSensor(){";
  js += "setStatus('distance-status',{key:'test_in_progress'},null);";
  js += "document.getElementById('distance-results').innerHTML='';";
  js += "const r=await fetch('/api/distance-sensor-test');";
  js += "const d=await r.json();";
  js += "if(d.success){";
  js += "let h='';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\" data-i18n=\"distance\">'+tr('distance')+'</div><div class=\"info-value\">'+d.distance.toFixed(2)+' cm</div></div>';";
  js += "document.getElementById('distance-results').innerHTML=h;";
  js += "}";
  js += "setStatus('distance-status',d.result,d.success?'success':'error');";
  js += "}";

  // Motion Sensor Functions
  js += "async function applyMotionConfig(){";
  js += "const pin=parseInt(document.getElementById('motionPin').value);";
  js += "const resp=await fetch('/api/motion-sensor-config?pin='+pin);";
  js += "const d=await resp.json();";
  js += "setStatus('motion-status',d.message,d.success?'success':'error');";
  js += "}";
  js += "async function testMotionSensor(){";
  js += "setStatus('motion-status',{key:'test_in_progress'},null);";
  js += "document.getElementById('motion-results').innerHTML='';";
  js += "const r=await fetch('/api/motion-sensor-test');";
  js += "const d=await r.json();";
  js += "if(d.success){";
  js += "let h='';";
  js += "const motionText=d.motion?tr('motion_detected'):tr('no_motion');";
  js += "const motionBadge=d.motion?'badge-warning':'badge-success';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">Status</div><div class=\"info-value\"><span class=\"badge '+motionBadge+'\">'+motionText+'</span></div></div>';";
  js += "document.getElementById('motion-results').innerHTML=h;";
  js += "}";
  js += "setStatus('motion-status',d.result,d.success?'success':'error');";
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

  js += "function setStatus(id,value,state){const el=document.getElementById(id);if(!el){return;}el.classList.remove('success','error');if(state){el.classList.add(state);}else{el.classList.remove('success','error');}if(value&&typeof value==='object'&&typeof value.key==='string'){setElementTranslation(el,value);}else if(value&&typeof value==='object'&&typeof value.text==='string'){clearTranslationAttributes(el);el.textContent=value.text;}else if(typeof value==='string'){clearTranslationAttributes(el);el.textContent=value;}else if(value==null){translateElement(el,getCurrentTranslations());}}"; 

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
  js += "if(d.bluetooth){";
  js += "const bt=d.bluetooth;";
  js += "const support=document.getElementById('overview-bt-support');";
  js += "if(support){const supportKey=bt.support_key||(bt.supported?'bluetooth_support_yes':'bluetooth_support_no');setElementTranslation(support,{key:supportKey});}";
  js += "const status=document.getElementById('overview-bt-status');";
  js += "if(status){if(bt.status_key){setElementTranslation(status,{key:bt.status_key});}else if(typeof bt.status==='string'){clearTranslationAttributes(status);status.textContent=bt.status;}}";
  js += "const name=document.getElementById('overview-bt-name');";
  js += "if(name){clearTranslationAttributes(name);name.textContent=bt.name||'';}";
  js += "const mac=document.getElementById('overview-bt-mac');";
  js += "if(mac){clearTranslationAttributes(mac);mac.textContent=bt.mac||'';}";
  js += "const advertising=document.getElementById('overview-bt-advertising');";
  js += "if(advertising){if(bt.advertising_key){setElementTranslation(advertising,{key:bt.advertising_key});}else if(typeof bt.advertising_label==='string'){clearTranslationAttributes(advertising);advertising.textContent=bt.advertising_label;}}";
  js += "const connection=document.getElementById('overview-bt-connection');";
  js += "if(connection){if(bt.connection_key){setElementTranslation(connection,{key:bt.connection_key});}else if(typeof bt.connection_label==='string'){clearTranslationAttributes(connection);connection.textContent=bt.connection_label;}}";
  js += "}";
  js += "}";

  // Changement de langue
  // --- [BUGFIX] Application immédiate des traductions lors du changement de langue ---
  js += "function changeLang(lang,btn){";
  js += "fetch('/api/set-language?lang='+lang,{cache:'no-store'}).then(r=>r.json()).then(d=>{";
  js += "if(!d.success){throw new Error('language switch rejected');}";
  js += "currentLang=lang;";
  js += "document.documentElement.setAttribute('lang',lang);";
  js += "document.querySelectorAll('.lang-btn').forEach(b=>b.classList.remove('active'));";
  js += "if(btn){btn.classList.add('active');}else{document.querySelectorAll('.lang-btn').forEach(b=>{if(b.getAttribute('data-lang')===lang){b.classList.add('active');}});}";
  js += "return fetchTranslations();";
  js += "}).then(translations=>{";
  js += "setTranslationsCache(translations);";
  js += "updateInterfaceTexts();";
  js += "const ind=document.getElementById('updateIndicator');if(ind){setElementTranslation(ind,{key:'language_updated',suffix:' ('+lang.toUpperCase()+')'});showUpdateIndicator();hideUpdateIndicator();}}";
  js += ").catch(err=>{const ind=document.getElementById('updateIndicator');if(ind){setElementTranslation(ind,{key:'language_switch_error',suffix:': '+String(err)});showUpdateIndicator();hideUpdateIndicator();}});";
  js += "}";

  return js;
}

#endif // WEB_INTERFACE_H