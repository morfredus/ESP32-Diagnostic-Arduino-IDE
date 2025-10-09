/*
 * WEB_INTERFACE.H - Interface Web Dynamique v3.0-dev
 * Gère l'interface HTML/CSS/JavaScript et la fonction handleRoot()
 */

#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

#include <Arduino.h>

// Déclaration de la fonction principale
void handleRoot();

// Implémentation de handleRoot()
void handleRoot() {
  // Générer et envoyer le HTML complet
  String html = F("<!DOCTYPE html><html lang='");
  html += (currentLanguage == LANG_FR) ? F("fr") : F("en");
  html += F("'><head>");
  html += F("<meta charset='UTF-8'>");
  html += F("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
  html += F("<title>ESP32 Diagnostic v");
  html += DIAGNOSTIC_VERSION;  // ✅ Changement
  html += F("</title>");

  // CSS
  html += F("<style>");
  html += F("*{margin:0;padding:0;box-sizing:border-box}");
  html += F("body{font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',sans-serif;");
  html += F("background:linear-gradient(135deg,#667eea 0%,#764ba2 100%);min-height:100vh;padding:20px}");
  html += F(".container{max-width:1400px;margin:0 auto;background:#fff;border-radius:20px;");
  html += F("box-shadow:0 20px 60px rgba(0,0,0,.3);overflow:hidden}");
  html += F(".header{background:linear-gradient(135deg,#667eea 0%,#764ba2 100%);");
  html += F("color:#fff;padding:30px;text-align:center;position:relative}");
  html += F(".header h1{font-size:2.5em;margin-bottom:10px;animation:fadeIn 0.5s}");
  html += F("@keyframes fadeIn{from{opacity:0;transform:translateY(-20px)}to{opacity:1;transform:translateY(0)}}");
  html += F(".lang-switcher{position:absolute;top:20px;right:20px;display:flex;gap:5px}");
  html += F(".lang-btn{padding:8px 15px;background:rgba(255,255,255,.2);border:2px solid rgba(255,255,255,.3);");
  html += F("border-radius:5px;color:#fff;cursor:pointer;font-weight:bold;transition:all .3s}");
  html += F(".lang-btn:hover{background:rgba(255,255,255,.3)}");
  html += F(".lang-btn.active{background:rgba(255,255,255,.4);border-color:rgba(255,255,255,.6)}");
  html += F(".status-indicator{display:inline-block;width:12px;height:12px;border-radius:50%;");
  html += F("margin-right:8px;animation:pulse 2s infinite}");
  html += F(".status-online{background:#0f0;box-shadow:0 0 10px #0f0}");
  html += F(".status-offline{background:#f00;box-shadow:0 0 10px #f00}");
  html += F("@keyframes pulse{0%,100%{opacity:1}50%{opacity:0.5}}");
  html += F(".nav{display:flex;justify-content:center;gap:10px;margin-top:20px;flex-wrap:wrap}");
  html += F(".nav-btn{padding:10px 20px;background:rgba(255,255,255,.2);border:none;");
  html += F("border-radius:5px;color:#fff;cursor:pointer;font-weight:bold;transition:all .3s}");
  html += F(".nav-btn:hover{background:rgba(255,255,255,.3);transform:translateY(-2px)}");
  html += F(".nav-btn.active{background:rgba(255,255,255,.4)}");
  html += F(".content{padding:30px}");
  html += F(".update-indicator{position:fixed;top:20px;right:20px;padding:10px 20px;");
  html += F("background:#667eea;color:#fff;border-radius:5px;font-weight:bold;opacity:0;");
  html += F("transition:opacity .3s;z-index:1000}");
  html += F(".update-indicator.show{opacity:1}");
  html += F(".loading{text-align:center;padding:50px;font-size:1.2em;color:#667eea}");
  html += F("</style>");
  html += F("</head><body>");

  // Indicateur de mise à jour
  html += F("<div class='update-indicator' id='updateIndicator'>Mise à jour...</div>");

  // Container principal
  html += F("<div class='container'>");
  html += F("<div class='header'>");

  // Sélecteur de langue
  html += F("<div class='lang-switcher'>");
  html += F("<button class='lang-btn ");
  html += (currentLanguage == LANG_FR) ? F("active") : F("");
  html += F("' onclick='changeLang(\"fr\")'>FR</button>");
  html += F("<button class='lang-btn ");
  html += (currentLanguage == LANG_EN) ? F("active") : F("");
  html += F("' onclick='changeLang(\"en\")'>EN</button>");
  html += F("</div>");

  // Titre avec indicateur de statut
  html += F("<h1 id='main-title'>");
  html += F("<span class='status-indicator status-online' id='statusIndicator'></span>");
  html += F("ESP32 Diagnostic v");
  html += DIAGNOSTIC_VERSION;  // ✅ Changement
  html += F("</h1>");

  html += F("<div style='font-size:1.2em;margin:10px 0' id='chipModel'>");
  html += diagnosticData.chipModel;
  html += F("</div>");

  html += F("<div style='font-size:.9em;opacity:.9;margin:10px 0'>");
  html += String(T().access) + F(": <a href='http://");
  html += MDNS_HOSTNAME;
  html += F(".local' style='color:#fff;text-decoration:underline'><strong>http://");
  html += MDNS_HOSTNAME;
  html += F(".local</strong></a> ") + String(T().or_text) + F(" <strong id='ipAddress'>");
  html += diagnosticData.ipAddress;
  html += F("</strong></div>");

  // Navigation
  html += F("<div class='nav'>");
  html += F("<button class='nav-btn active' onclick='showTab(\"overview\")' data-i18n='nav_overview'>");
  html += String(T().nav_overview) + F("</button>");
  html += F("<button class='nav-btn' onclick='showTab(\"leds\")' data-i18n='nav_leds'>");
  html += String(T().nav_leds) + F("</button>");
  html += F("<button class='nav-btn' onclick='showTab(\"screens\")' data-i18n='nav_screens'>");
  html += String(T().nav_screens) + F("</button>");
  html += F("<button class='nav-btn' onclick='showTab(\"tests\")' data-i18n='nav_tests'>");
  html += String(T().nav_tests) + F("</button>");
  html += F("<button class='nav-btn' onclick='showTab(\"gpio\")' data-i18n='nav_gpio'>");
  html += String(T().nav_gpio) + F("</button>");
  html += F("<button class='nav-btn' onclick='showTab(\"wifi\")' data-i18n='nav_wifi'>");
  html += String(T().nav_wifi) + F("</button>");
  html += F("<button class='nav-btn' onclick='showTab(\"benchmark\")' data-i18n='nav_benchmark'>");
  html += String(T().nav_benchmark) + F("</button>");
  html += F("<button class='nav-btn' onclick='showTab(\"export\")' data-i18n='nav_export'>");
  html += String(T().nav_export) + F("</button>");
  html += F("</div>");

  html += F("</div>"); // Fin header

  // Content avec message de chargement
  html += F("<div class='content'>");
  html += F("<div class='loading'>⏳ Chargement de l'interface dynamique...</div>");
  html += F("</div>");

  html += F("</div>"); // Fin container

  // JavaScript minifié
  html += F("<script>");
  html += F("const UPDATE_INTERVAL=5000;");
  html += F("let currentLang='");
  html += (currentLanguage == LANG_FR) ? F("fr") : F("en");
  html += F("';");
  html += F("let updateTimer=null;");
  html += F("let isConnected=true;");

  // Fonction principaleexpo
  html += F("document.addEventListener('DOMContentLoaded',function(){");
  html += F("console.log('ESP32 Diagnostic v");
  html += DIAGNOSTIC_VERSION;  // ✅ Changement
  html += F(" - Init');");
  html += F("document.querySelector('.content').innerHTML='<p style=\"text-align:center;padding:50px\">Interface dynamique chargée!<br><br>Utilisez les onglets ci-dessus pour accéder aux différentes fonctionnalités.</p>';");
  html += F("startAutoUpdate();");
  html += F("});");

  // Auto-update
  html += F("function startAutoUpdate(){");
  html += F("if(updateTimer)clearInterval(updateTimer);");
  html += F("updateTimer=setInterval(()=>{");
  html += F("if(isConnected)updateLiveData();");
  html += F("},UPDATE_INTERVAL);");
  html += F("}");

  // Mise à jour données live
  html += F("function updateLiveData(){");
  html += F("fetch('/api/status').then(r=>r.json()).then(d=>{");
  html += F("isConnected=true;");
  html += F("document.getElementById('statusIndicator').className='status-indicator status-online';");
  html += F("}).catch(e=>{");
  html += F("console.error('Erreur:',e);");
  html += F("isConnected=false;");
  html += F("document.getElementById('statusIndicator').className='status-indicator status-offline';");
  html += F("});");
  html += F("}");

  // Navigation onglets
  html += F("function showTab(t){");
  html += F("document.querySelectorAll('.nav-btn').forEach(b=>b.classList.remove('active'));");
  html += F("event.target.classList.add('active');");
  html += F("document.querySelector('.content').innerHTML='<div class=\"loading\">⏳ Chargement...</div>';");
  html += F("loadTabContent(t);");
  html += F("}");

  // Chargement contenu onglet
  html += F("function loadTabContent(tab){");
  html += F("let content='<p style=\"text-align:center;padding:50px\">';");
  html += F("if(tab==='overview')content+='📊 Vue d\\'ensemble du système';");
  html += F("else if(tab==='leds')content+='💡 Contrôle des LEDs';");
  html += F("else if(tab==='screens')content+='📺 Tests des écrans';");
  html += F("else if(tab==='tests')content+='🧪 Tests matériels avancés';");
  html += F("else if(tab==='gpio')content+='🔌 Tests GPIO';");
  html += F("else if(tab==='wifi')content+='📡 Scanner WiFi';");
  html += F("else if(tab==='benchmark')content+='⚡ Benchmarks de performance';");
  html += F("else if(tab==='export')content+='📤 Export des données';");
  html += F("content+='</p><p style=\"text-align:center\">Utilisez les routes API pour accéder aux fonctionnalités:<br>';");
  html += F("content+='<code>/api/test-gpio</code>, <code>/api/wifi-scan</code>, etc.</p>';");
  html += F("document.querySelector('.content').innerHTML=content;");
  html += F("}");

  // Changement langue
  html += F("function changeLang(lang){");
  html += F("fetch('/api/set-language?lang='+lang).then(r=>r.json()).then(d=>{");
  html += F("if(d.success){");
  html += F("currentLang=lang;");
  html += F("document.querySelectorAll('.lang-btn').forEach(b=>b.classList.remove('active'));");
  html += F("event.target.classList.add('active');");
  html += F("location.reload();");
  html += F("}});");
  html += F("}");

  html += F("</script>");
  html += F("</body></html>");

  // Envoyer le HTML
  server.send(200, "text/html; charset=utf-8", html);
}

// Fonction pour générer le JavaScript (version simplifiée)
String generateJavaScript() {
  return F("console.log('ESP32 Diagnostic v3.0-dev API Ready');");
}

#endif // WEB_INTERFACE_H
