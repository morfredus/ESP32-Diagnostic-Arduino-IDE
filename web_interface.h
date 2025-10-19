/*
 * WEB_INTERFACE.H - Interface Web Dynamique v4.1.0
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
extern String getWiFiSignalQuality();

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
  html += R"CSS(
:root {
  --bg-gradient: linear-gradient(135deg, #0b1220 0%, #182b4d 45%, #1f3a70 100%);
  --card-bg: rgba(15, 23, 42, 0.72);
  --card-border: rgba(255, 255, 255, 0.08);
  --accent: #38bdf8;
  --accent-strong: #0ea5e9;
  --danger: #f87171;
  --success: #34d399;
  --warning: #fbbf24;
  --surface: rgba(148, 163, 184, 0.08);
  --text-primary: #f8fafc;
  --text-secondary: rgba(226, 232, 240, 0.82);
  --muted: rgba(148, 163, 184, 0.7);
  --shadow-xl: 0 20px 45px rgba(8, 15, 35, 0.55);
}

* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

body {
  font-family: 'Inter', -apple-system, BlinkMacSystemFont, 'Segoe UI', sans-serif;
  background: var(--bg-gradient);
  color: var(--text-primary);
  min-height: 100vh;
  padding: 28px;
  display: flex;
  justify-content: center;
  position: relative;
  overflow-x: hidden;
}

body::before,
body::after {
  content: '';
  position: fixed;
  width: 420px;
  height: 420px;
  border-radius: 50%;
  background: radial-gradient(circle at center, rgba(14, 165, 233, 0.28), transparent 65%);
  z-index: -1;
  opacity: 0.55;
  transition: transform 0.6s ease;
}

body::before {
  top: -160px;
  left: -120px;
}

body::after {
  bottom: -180px;
  right: -90px;
  background: radial-gradient(circle at center, rgba(56, 189, 248, 0.22), transparent 60%);
}

a {
  color: var(--accent);
  text-decoration: none;
}

a:hover {
  color: var(--accent-strong);
}

.app-wrapper {
  width: min(1200px, 100%);
  display: flex;
  flex-direction: column;
  gap: 28px;
  position: relative;
}

.app-header {
  position: relative;
  overflow: hidden;
  background: var(--card-bg);
  border: 1px solid var(--card-border);
  border-radius: 26px;
  padding: 32px;
  box-shadow: var(--shadow-xl);
  backdrop-filter: blur(14px);
  display: flex;
  flex-direction: column;
  gap: 28px;
}

.app-header::after {
  content: '';
  position: absolute;
  inset: 0;
  pointer-events: none;
  background: radial-gradient(circle at top right, rgba(56, 189, 248, 0.22), transparent 55%);
  opacity: 0.85;
}

.header-main {
  display: flex;
  flex-wrap: wrap;
  align-items: center;
  justify-content: space-between;
  gap: 16px;
}

.brand {
  display: flex;
  align-items: center;
  gap: 16px;
}

.brand h1 {
  display: flex;
  align-items: baseline;
  gap: 12px;
  font-size: clamp(1.8rem, 2vw + 1.2rem, 2.8rem);
  font-weight: 700;
}

.version-inline {
  font-size: 0.58em;
  padding: 6px 14px;
  border-radius: 999px;
  background: rgba(56, 189, 248, 0.18);
  color: var(--accent);
  letter-spacing: 0.12em;
  text-transform: uppercase;
  font-weight: 600;
}

.status-indicator {
  width: 14px;
  height: 14px;
  border-radius: 999px;
  display: inline-block;
  box-shadow: 0 0 15px rgba(34, 197, 94, 0.8);
  animation: pulse 2.4s infinite;
}

.status-online {
  background: linear-gradient(135deg, #22c55e, #4ade80);
}

.status-offline {
  background: linear-gradient(135deg, #f87171, #ef4444);
  box-shadow: 0 0 18px rgba(248, 113, 113, 0.8);
}

@keyframes pulse {
  0%, 100% {
    transform: scale(1);
    opacity: 1;
  }
  50% {
    transform: scale(1.15);
    opacity: 0.6;
  }
}

.lang-switcher {
  display: flex;
  gap: 10px;
}

.lang-btn {
  padding: 10px 18px;
  border-radius: 14px;
  border: 1px solid rgba(148, 163, 184, 0.3);
  background: rgba(15, 23, 42, 0.45);
  color: var(--text-secondary);
  font-weight: 600;
  letter-spacing: 0.08em;
  transition: all 0.25s ease;
  cursor: pointer;
}

.lang-btn:hover {
  color: var(--text-primary);
  border-color: rgba(56, 189, 248, 0.6);
}

.lang-btn.active {
  background: linear-gradient(135deg, rgba(14, 165, 233, 0.85), rgba(56, 189, 248, 0.6));
  color: #0f172a;
  border-color: transparent;
}

.header-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(220px, 1fr));
  gap: 18px;
}

.header-summary {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(220px, 1fr));
  gap: 18px;
}

.summary-card {
  position: relative;
  display: flex;
  align-items: center;
  gap: 16px;
  padding: 20px 22px;
  border-radius: 20px;
  background: rgba(15, 23, 42, 0.55);
  border: 1px solid rgba(148, 163, 184, 0.24);
  box-shadow: inset 0 0 0 1px rgba(148, 163, 184, 0.08);
  transition: transform 0.3s ease, border-color 0.3s ease, box-shadow 0.3s ease;
}

.summary-card::after {
  content: '';
  position: absolute;
  inset: 0;
  background: linear-gradient(135deg, rgba(14, 165, 233, 0.18), transparent 70%);
  opacity: 0;
  pointer-events: none;
  z-index: -1;
  transition: opacity 0.3s ease;
}

.summary-card:hover {
  transform: translateY(-4px);
  border-color: rgba(56, 189, 248, 0.45);
  box-shadow: 0 18px 38px rgba(8, 15, 35, 0.32);
}

.summary-card:hover::after {
  opacity: 1;
}

.summary-icon {
  position: relative;
  font-size: 1.8rem;
  z-index: 1;
}

.summary-content {
  position: relative;
  display: flex;
  flex-direction: column;
  gap: 4px;
  z-index: 1;
}

.summary-label {
  font-size: 0.72rem;
  text-transform: uppercase;
  letter-spacing: 0.16em;
  color: var(--muted);
  font-weight: 700;
}

.summary-value {
  font-size: 1.18rem;
  font-weight: 600;
  color: var(--text-primary);
}

.meta-card {
  background: var(--surface);
  border-radius: 18px;
  border: 1px solid rgba(148, 163, 184, 0.2);
  padding: 22px;
  display: flex;
  flex-direction: column;
  gap: 8px;
  min-height: 132px;
  position: relative;
  overflow: hidden;
  transition: transform 0.3s ease, border-color 0.3s ease, box-shadow 0.3s ease;
}

.meta-card::after {
  content: '';
  position: absolute;
  inset: 0;
  background: radial-gradient(circle at top right, rgba(56, 189, 248, 0.18), transparent 60%);
  opacity: 0;
  pointer-events: none;
  z-index: -1;
  transition: opacity 0.3s ease;
}

.meta-card:hover {
  transform: translateY(-6px);
  border-color: rgba(56, 189, 248, 0.45);
  box-shadow: 0 22px 45px rgba(8, 15, 35, 0.35);
}

.meta-card:hover::after {
  opacity: 1;
}

.meta-card a {
  color: var(--accent);
}

.meta-label {
  text-transform: uppercase;
  letter-spacing: 0.14em;
  font-size: 0.75rem;
  color: var(--muted);
  font-weight: 600;
}

.meta-value {
  font-size: 1.15rem;
  font-weight: 600;
  color: var(--text-primary);
  word-break: break-word;
}

.meta-sub {
  font-size: 0.9rem;
  color: var(--text-secondary);
}

.status-card {
  background: linear-gradient(135deg, rgba(14, 165, 233, 0.22), rgba(14, 116, 233, 0.08));
  border-color: rgba(56, 189, 248, 0.3);
}

.status-text.offline {
  color: var(--warning);
}

.app-body {
  display: flex;
  flex-direction: column;
  gap: 28px;
}

.app-nav {
  background: var(--card-bg);
  border: 1px solid var(--card-border);
  border-radius: 22px;
  padding: 24px;
  box-shadow: var(--shadow-xl);
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(160px, 1fr));
  gap: 14px;
  backdrop-filter: blur(16px);
}

.nav-btn {
  padding: 14px 16px;
  border-radius: 16px;
  border: 1px solid transparent;
  background: rgba(15, 23, 42, 0.45);
  color: var(--text-secondary);
  font-weight: 600;
  font-size: 0.95rem;
  letter-spacing: 0.02em;
  cursor: pointer;
  transition: all 0.25s ease;
  text-align: center;
  display: inline-flex;
  align-items: center;
  justify-content: center;
  gap: 10px;
}

.nav-btn:hover {
  color: var(--text-primary);
  border-color: rgba(56, 189, 248, 0.5);
  transform: translateY(-2px);
}

.nav-btn::before {
  content: attr(data-icon);
  font-size: 1.15rem;
}

.nav-btn.active {
  background: linear-gradient(135deg, rgba(14, 165, 233, 0.9), rgba(56, 189, 248, 0.55));
  color: #0b1220;
  border-color: transparent;
  box-shadow: 0 16px 30px rgba(14, 165, 233, 0.25);
}

.app-main {
  background: var(--card-bg);
  border: 1px solid var(--card-border);
  border-radius: 26px;
  padding: 34px;
  box-shadow: var(--shadow-xl);
  backdrop-filter: blur(18px);
}

.tab-container {
  display: flex;
  flex-direction: column;
  gap: 26px;
}

.content {
  display: contents;
}

.tab-content {
  display: none;
  animation: fadeIn 0.45s ease;
}

.tab-content.active {
  display: block;
}

@keyframes fadeIn {
  from {
    opacity: 0;
    transform: translateY(14px);
  }
  to {
    opacity: 1;
    transform: translateY(0);
  }
}

.section {
  background: rgba(15, 23, 42, 0.6);
  border-radius: 22px;
  border: 1px solid rgba(148, 163, 184, 0.14);
  padding: 26px;
  display: flex;
  flex-direction: column;
  gap: 18px;
  box-shadow: inset 0 0 0 1px rgba(148, 163, 184, 0.06);
}

.section h2 {
  display: flex;
  align-items: center;
  gap: 12px;
  font-size: 1.4rem;
  color: var(--accent);
}

.section h3 {
  font-size: 1.1rem;
  color: var(--text-secondary);
  margin-top: 4px;
}

.info-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(220px, 1fr));
  gap: 16px;
}

.info-item {
  background: rgba(15, 23, 42, 0.55);
  border-radius: 16px;
  border: 1px solid rgba(148, 163, 184, 0.2);
  padding: 18px;
  transition: transform 0.25s ease, border-color 0.25s ease, box-shadow 0.25s ease;
}

.info-item:hover {
  transform: translateY(-4px);
  border-color: rgba(56, 189, 248, 0.5);
  box-shadow: 0 18px 30px rgba(14, 165, 233, 0.18);
}

.info-label {
  font-weight: 700;
  color: var(--muted);
  text-transform: uppercase;
  letter-spacing: 0.08em;
  font-size: 0.8rem;
  margin-bottom: 8px;
}

.info-value {
  font-weight: 600;
  font-size: 1.05rem;
  color: var(--text-primary);
}

.status-list {
  list-style: none;
  display: flex;
  flex-direction: column;
  gap: 10px;
  margin: 0;
  padding: 0;
}

.status-list li {
  display: flex;
  align-items: center;
  gap: 12px;
  color: var(--text-secondary);
}

.status-pill {
  display: inline-flex;
  align-items: center;
  justify-content: center;
  width: 30px;
  height: 30px;
  border-radius: 999px;
  font-size: 1.1rem;
  font-weight: 700;
}

.status-pill.ok {
  background: rgba(52, 211, 153, 0.2);
  color: #34d399;
}

.status-pill.warn {
  background: rgba(251, 191, 36, 0.2);
  color: #fbbf24;
}

.status-pill.ko {
  background: rgba(248, 113, 113, 0.2);
  color: #f87171;
}

.wireless-summary {
  display: grid;
  gap: 18px;
}

.wireless-card {
  margin: 0;
}

.wireless-card-title {
  font-weight: 700;
  color: var(--accent);
  text-transform: uppercase;
  letter-spacing: 0.08em;
  font-size: 0.9rem;
}

.wireless-status {
  font-size: 1.05rem;
  font-weight: 600;
  color: var(--text-primary);
}

.wireless-list {
  list-style: none;
  display: flex;
  flex-direction: column;
  gap: 10px;
  padding: 0;
  margin: 0;
}

.wireless-list li {
  display: flex;
  align-items: center;
  gap: 12px;
  padding: 12px 14px;
  border-radius: 14px;
  background: rgba(15, 23, 42, 0.45);
  border: 1px solid rgba(148, 163, 184, 0.24);
}

.wireless-list .label {
  flex: 1;
  font-weight: 600;
  color: var(--muted);
}

.wireless-list .value {
  font-weight: 600;
  color: var(--text-primary);
}

.wireless-empty {
  font-style: italic;
  color: var(--muted);
  border-style: dashed;
  border-color: rgba(148, 163, 184, 0.3);
}

.wireless-hint {
  margin-top: 12px;
  padding: 16px;
  border-radius: 14px;
  background: rgba(251, 191, 36, 0.12);
  border-left: 4px solid rgba(251, 191, 36, 0.5);
  color: var(--text-primary);
  font-size: 0.95rem;
}

.badge {
  display: inline-block;
  padding: 6px 16px;
  border-radius: 999px;
  font-size: 0.85rem;
  font-weight: 600;
}

.badge-success {
  background: rgba(52, 211, 153, 0.22);
  color: #34d399;
}

.badge-warning {
  background: rgba(251, 191, 36, 0.22);
  color: #fbbf24;
}

.badge-danger {
  background: rgba(248, 113, 113, 0.22);
  color: #f87171;
}

.badge-info {
  background: rgba(59, 130, 246, 0.22);
  color: #60a5fa;
}

.btn {
  padding: 12px 20px;
  border-radius: 14px;
  border: none;
  font-size: 0.95rem;
  font-weight: 600;
  cursor: pointer;
  margin: 6px;
  transition: transform 0.2s ease, box-shadow 0.2s ease, opacity 0.2s ease;
  display: inline-flex;
  align-items: center;
  gap: 10px;
  justify-content: center;
}

.btn:hover {
  transform: translateY(-2px);
  box-shadow: 0 12px 20px rgba(14, 165, 233, 0.18);
}

.btn:disabled,
.btn[disabled] {
  opacity: 0.5;
  cursor: not-allowed;
  box-shadow: none;
  transform: none;
}

.btn-primary {
  background: linear-gradient(135deg, #1d4ed8, #38bdf8);
  color: #f8fafc;
}

.btn-success {
  background: linear-gradient(135deg, #059669, #34d399);
  color: #f0fdf4;
}

.btn-info {
  background: linear-gradient(135deg, #0284c7, #38bdf8);
  color: #f0f9ff;
}

.btn-danger {
  background: linear-gradient(135deg, #dc2626, #f87171);
  color: #fff5f5;
}

.btn-warning {
  background: linear-gradient(135deg, #ea580c, #fbbf24);
  color: #fff7ed;
}

.progress-bar {
  background: rgba(148, 163, 184, 0.2);
  border-radius: 14px;
  height: 26px;
  overflow: hidden;
  position: relative;
}

.progress-fill {
  height: 100%;
  background: linear-gradient(90deg, #38bdf8, #0ea5e9);
  display: flex;
  align-items: center;
  justify-content: center;
  color: #0b1220;
  font-weight: 700;
  transition: width 0.5s ease;
}

.card {
  background: rgba(15, 23, 42, 0.55);
  border-radius: 18px;
  border: 1px solid rgba(148, 163, 184, 0.2);
  padding: 20px;
  transition: transform 0.2s ease, border-color 0.2s ease;
}

.card:hover {
  transform: translateY(-4px);
  border-color: rgba(56, 189, 248, 0.5);
}

.loading {
  width: 24px;
  height: 24px;
  border: 3px solid rgba(148, 163, 184, 0.3);
  border-top-color: var(--accent);
  border-radius: 999px;
  animation: spin 1s linear infinite;
  margin: 0 auto;
}

@keyframes spin {
  from {
    transform: rotate(0deg);
  }
  to {
    transform: rotate(360deg);
  }
}

.status-live {
  padding: 18px;
  border-radius: 16px;
  border-left: 4px solid var(--accent);
  background: rgba(56, 189, 248, 0.1);
  text-align: center;
  font-weight: 600;
  color: var(--text-secondary);
}

.inline-feedback {
  margin-top: 10px;
  font-size: 0.9rem;
  opacity: 0;
  transition: opacity 0.3s ease;
}

.inline-feedback.show {
  opacity: 1;
}

.inline-feedback.success {
  color: #34d399;
}

.inline-feedback.error {
  color: #f87171;
}

.inline-feedback.info {
  color: var(--text-secondary);
}

.gpio-grid {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(90px, 1fr));
  gap: 12px;
}

.gpio-item {
  padding: 16px;
  border-radius: 12px;
  text-align: center;
  font-weight: 700;
  border: 1px solid rgba(148, 163, 184, 0.3);
  background: rgba(15, 23, 42, 0.55);
  transition: transform 0.2s ease, border-color 0.2s ease;
}

.gpio-item:hover {
  transform: translateY(-3px);
}

.gpio-ok {
  border-color: rgba(52, 211, 153, 0.6);
  background: rgba(52, 211, 153, 0.12);
}

.gpio-fail {
  border-color: rgba(248, 113, 113, 0.6);
  background: rgba(248, 113, 113, 0.12);
}

.wifi-list,
.ble-list {
  max-height: 480px;
  overflow-y: auto;
  display: grid;
  gap: 14px;
  padding-right: 4px;
}

.wifi-item,
.ble-item {
  background: rgba(15, 23, 42, 0.55);
  border-radius: 16px;
  border: 1px solid rgba(148, 163, 184, 0.24);
  padding: 18px;
  transition: transform 0.2s ease, border-color 0.2s ease;
}

.wifi-item:hover,
.ble-item:hover {
  transform: translateY(-4px);
  border-color: rgba(56, 189, 248, 0.5);
}

.ble-item {
  border-left: 4px solid rgba(14, 165, 233, 0.6);
}

.wifi-item {
  border-left: 4px solid rgba(56, 189, 248, 0.6);
}

input[type='number'],
input[type='color'],
input[type='text'] {
  padding: 12px;
  border-radius: 14px;
  border: 1px solid rgba(148, 163, 184, 0.3);
  background: rgba(15, 23, 42, 0.6);
  color: var(--text-primary);
  font-size: 0.95rem;
  transition: border-color 0.2s ease, box-shadow 0.2s ease;
}

input:focus {
  outline: none;
  border-color: rgba(56, 189, 248, 0.6);
  box-shadow: 0 0 0 3px rgba(14, 165, 233, 0.2);
}

.update-indicator {
  position: fixed;
  top: 20px;
  right: 24px;
  padding: 12px 22px;
  border-radius: 14px;
  background: linear-gradient(135deg, rgba(14, 165, 233, 0.92), rgba(56, 189, 248, 0.7));
  color: #0b1220;
  font-weight: 700;
  letter-spacing: 0.04em;
  opacity: 0;
  transition: opacity 0.3s ease, transform 0.3s ease;
  transform: translateY(-12px);
  z-index: 1000;
}

.update-indicator.show {
  opacity: 1;
  transform: translateY(0);
}

@media (max-width: 900px) {
  body {
    padding: 18px;
  }

  .app-nav {
    grid-template-columns: repeat(auto-fit, minmax(140px, 1fr));
  }

  .header-summary {
    grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
  }

  .app-main,
  .app-header {
    padding: 26px;
  }
}

@media (max-width: 640px) {
  body {
    padding: 16px 12px;
  }

  .lang-switcher {
    width: 100%;
    justify-content: center;
  }

  .header-main {
    flex-direction: column;
    align-items: flex-start;
  }

  .app-nav {
    overflow-x: auto;
    display: flex;
    flex-wrap: nowrap;
    gap: 10px;
  }

  .nav-btn {
    flex: 1 0 auto;
    min-width: 160px;
  }

  .header-summary {
    grid-template-columns: 1fr;
  }
}

@media print {
  body {
    background: #fff;
    color: #000;
  }

  .update-indicator,
  .lang-switcher,
  .app-nav,
  .btn,
  .status-indicator {
    display: none !important;
  }

  .app-wrapper,
  .app-header,
  .app-main,
  .section,
  .info-item {
    background: #fff;
    border: 1px solid #ccc;
    box-shadow: none;
    color: #000;
  }
}
)CSS";
  html += "</style>";
  html += "</head><body>";
  html += "<div class='update-indicator' id='updateIndicator'>Mise à jour...</div>";
  html += "<div class='app-wrapper'>";
  html += "<header class='app-header'>";
  html += "<div class='header-main'>";
  html += "<div class='brand'>";
  html += "<span class='status-indicator status-online' id='statusIndicator'></span>";
  html += "<h1 id='main-title'>Diagnostic ESP32 <span class='version-inline'>v"; 
  html += DIAGNOSTIC_VERSION_STR;
  html += "</span></h1>";
  html += "</div>";
  html += "<div class='lang-switcher'>";
  html += "<button class='lang-btn active' onclick='changeLang(\"fr\")'>FR</button>";
  html += "<button class='lang-btn' onclick='changeLang(\"en\")'>EN</button>";
  html += "</div>";
  html += "</div>";
  html += "<div class='header-summary'>";
  html += "<div class='summary-card'>";
  html += "<span class='summary-icon'>🧠</span>";
  html += "<div class='summary-content'>";
  html += "<span class='summary-label' data-i18n='chip_info'>Informations Chip Détaillées</span>";
  html += "<span class='summary-value' id='summaryChip'>";
  html += diagnosticData.chipModel;
  html += "</span>";
  html += "</div></div>";
  html += "<div class='summary-card'>";
  html += "<span class='summary-icon'>📶</span>";
  html += "<div class='summary-content'>";
  html += "<span class='summary-label' data-i18n='wifi_connection'>Connexion WiFi Détaillée</span>";
  html += "<span class='summary-value' id='summaryNetwork'>";
  if (diagnosticData.wifiSSID.length()) {
    html += diagnosticData.wifiSSID;
  } else {
    html += "—";
  }
  html += "</span>";
  html += "</div></div>";
  html += "<div class='summary-card'>";
  html += "<span class='summary-icon'>⏱️</span>";
  html += "<div class='summary-content'>";
  html += "<span class='summary-label' data-i18n='uptime'>Uptime</span>";
  html += "<span class='summary-value' id='summaryUptime'>—</span>";
  html += "</div></div>";
  html += "</div>";
  html += "<div class='header-grid'>";
  html += "<div class='meta-card'>";
  html += "<span class='meta-label'>Modèle de puce</span>";
  html += "<span class='meta-value' id='chipModel'>";
  html += diagnosticData.chipModel;
  html += "</span>";
  html += "</div>";
  html += "<div class='meta-card'>";
  html += "<span class='meta-label'>Accès réseau</span>";
  html += "<span class='meta-value'><a href='http://";
  html += MDNS_HOSTNAME_STR;
  html += ".local' target='_blank' rel='noopener'>http://";
  html += MDNS_HOSTNAME_STR;
  html += ".local</a></span>";
  html += "<span class='meta-sub'><span data-i18n='ip_address'>Adresse IP</span> : <strong id='ipAddress'>";
  if (diagnosticData.ipAddress.length()) {
    html += diagnosticData.ipAddress;
  } else {
    html += "—";
  }
  html += "</strong></span>";
  html += "<span class='meta-sub'><span data-i18n='signal_quality'>Qualité Signal</span> : <strong id='networkQuality'>";
  html += getWiFiSignalQuality();
  html += "</strong></span>";
  html += "</div>";
  html += "<div class='meta-card status-card'>";
  html += "<span class='meta-label'>Statut du service</span>";
  html += "<span class='meta-value status-text' id='serviceStatus'>Connecté</span>";
  html += "<span class='meta-sub'>Les données se rafraîchissent automatiquement.</span>";
  html += "</div>";
  html += "</div>";
  html += "</header>";
  html += "<div class='app-body'>";
  html += "<nav class='app-nav'>";
  html += "<button class='nav-btn active' data-icon='🏠' onclick='showTab(\"overview\",event)'>Vue d'ensemble</button>";
  html += "<button class='nav-btn' data-icon='💡' onclick='showTab(\"leds\",event)'>LEDs</button>";
  html += "<button class='nav-btn' data-icon='🖥️' onclick='showTab(\"screens\",event)'>Écrans</button>";
  html += "<button class='nav-btn' data-icon='🧪' onclick='showTab(\"tests\",event)'>Tests</button>";
  html += "<button class='nav-btn' data-icon='🔌' onclick='showTab(\"gpio\",event)'>GPIO</button>";
  html += "<button class='nav-btn' data-icon='📶' onclick='showTab(\"wifi\",event)'>Sans fil</button>";
  html += "<button class='nav-btn' data-icon='⚡' onclick='showTab(\"benchmark\",event)'>Performance</button>";
  html += "<button class='nav-btn' data-icon='📤' onclick='showTab(\"export\",event)'>Export</button>";
  html += "</nav>";
  html += "<main class='app-main'>";
  html += "<div class='content'>";
  html += "<div id='tabContainer' class='tab-container'></div>";
  html += "</div>";
  html += "</main>";
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
  js += "const [overviewRes,wirelessRes]=await Promise.all([fetch('/api/overview'),fetch('/api/wireless-status')]);";
  js += "const overview=await overviewRes.json();";
  js += "const wireless=await wirelessRes.json();";
  js += "applyOverviewData(overview);";
  js += "updateRealtimeValues(overview);";
  js += "updateWirelessSummary(wireless);";
  js += "isConnected=true;updateStatusIndicator(true);";
  js += "}catch(error){console.error('Erreur:',error);isConnected=false;updateStatusIndicator(false);}";
  js += "hideUpdateIndicator();";
  js += "}";

  // Live data
  js += "async function updateLiveData(){";
  js += "try{";
  js += "const [overviewRes,wirelessRes]=await Promise.all([fetch('/api/overview'),fetch('/api/wireless-status')]);";
  js += "const overview=await overviewRes.json();";
  js += "const wireless=await wirelessRes.json();";
  js += "applyOverviewData(overview);";
  js += "updateRealtimeValues(overview);";
  js += "updateWirelessSummary(wireless);";
  js += "isConnected=true;updateStatusIndicator(true);";
  js += "}catch(error){console.error('Erreur:',error);isConnected=false;updateStatusIndicator(false);}";
  js += "}";

  js += "function applyOverviewData(data){";
  js += "if(!data)return;";
  js += "const chip=data.chip||{};";
  js += "const wifi=data.wifi||{};";
  js += "const chipEl=document.getElementById('chipModel');";
  js += "if(chipEl)chipEl.textContent=chip.model||'—';";
  js += "const ipEl=document.getElementById('ipAddress');";
  js += "if(ipEl)ipEl.textContent=(wifi.ip&&wifi.ip.length)?wifi.ip:'—';";
  js += "const summaryChip=document.getElementById('summaryChip');";
  js += "if(summaryChip)summaryChip.textContent=chip.model||'—';";
  js += "const summaryNetwork=document.getElementById('summaryNetwork');";
  js += "if(summaryNetwork)summaryNetwork.textContent=(wifi.ssid&&wifi.ssid.length)?wifi.ssid:(translations.wifi_not_connected||'Non connecté');";
  js += "const summaryUptime=document.getElementById('summaryUptime');";
  js += "if(summaryUptime&&typeof chip.uptime!=='undefined')summaryUptime.textContent=formatUptime(chip.uptime);";
  js += "const qualityEl=document.getElementById('networkQuality');";
  js += "if(qualityEl)qualityEl.textContent=wifi.quality||'—';";
  js += "}";

  js += "async function loadWirelessStatus(){try{const r=await fetch('/api/wireless-status');const d=await r.json();updateWirelessSummary(d);}catch(e){console.error('wireless-status',e);}}";
  js += R"JS(function updateWirelessSummary(data) {
    const wifiData = (data && data.wifi) ? data.wifi : {};
    const wifiStatus = document.getElementById('wifi-summary-status');
    if (wifiStatus) {
      if (wifiData.connected) {
        const baseLabel = (translations.connected || 'Connecté');
        const ssidLabel = wifiData.ssid ? ' · ' + wifiData.ssid : '';
        wifiStatus.textContent = baseLabel + ssidLabel;
      } else {
        wifiStatus.textContent = translations.wifi_not_connected || 'Non connecté';
      }
    }

    const summaryNetwork = document.getElementById('summaryNetwork');
    if (summaryNetwork) {
      summaryNetwork.textContent = wifiData.connected
        ? (wifiData.ssid || '—')
        : (translations.wifi_not_connected || 'Non connecté');
    }
    const qualityChip = document.getElementById('networkQuality');
    if (qualityChip) {
      qualityChip.textContent = wifiData.quality || '—';
    }

    const wifiDetails = document.getElementById('wifi-summary-details');
    if (wifiDetails) {
      wifiDetails.innerHTML = '';
      if (wifiData.connected) {
        const rows = [
          { label: translations.connected_ssid || 'SSID connecté', value: wifiData.ssid || '—' },
          { label: translations.signal_power || 'Puissance Signal (RSSI)', value: (typeof wifiData.rssi === 'number' ? wifiData.rssi + ' dBm' : '—') },
          { label: translations.signal_quality || 'Qualité Signal', value: wifiData.quality || '—' },
          { label: translations.ip_address || 'Adresse IP', value: wifiData.ip || '—' },
          { label: translations.subnet_mask || 'Masque Sous-réseau', value: wifiData.subnet || '—' },
          { label: translations.gateway || 'Passerelle', value: wifiData.gateway || '—' },
          { label: translations.dns || 'DNS', value: wifiData.dns || '—' }
        ];
        rows.filter(item => item.value && item.value !== '—').forEach(item => {
          const li = document.createElement('li');
          const label = document.createElement('span');
          label.className = 'label';
          label.textContent = item.label;
          const value = document.createElement('span');
          value.className = 'value';
          value.textContent = item.value;
          li.appendChild(label);
          li.appendChild(value);
          wifiDetails.appendChild(li);
        });
        if (!wifiDetails.children.length) {
          const li = document.createElement('li');
          li.className = 'wireless-empty';
          li.textContent = '—';
          wifiDetails.appendChild(li);
        }
      } else {
        const li = document.createElement('li');
        li.className = 'wireless-empty';
        li.textContent = translations.wifi_not_connected || 'Non connecté';
        wifiDetails.appendChild(li);
      }
    }

    const bleData = (data && data.ble) ? data.ble : {};
    const bleStatus = document.getElementById('ble-summary-status');
    if (bleStatus) {
      bleStatus.textContent = bleData.status || (translations.ble_not_supported || 'Bluetooth LE non disponible');
    }

    const bleDetails = document.getElementById('ble-summary-details');
    if (bleDetails) {
      bleDetails.innerHTML = '';
      const yesText = translations.status_yes || 'Oui';
      const noText = translations.status_no || 'Non';
      const missingText = translations.status_missing || 'Manquant';
      const chipLabel = translations.ble_chip_support || 'Compatibilité matérielle BLE';
      const stackLabel = translations.ble_stack_support || 'Pile BLE incluse dans ce firmware';
      const runtimeLabel = translations.ble_runtime_status || 'Fonctions BLE actives';
      const entries = [
        { className: bleData.chipCapable ? 'ok' : 'ko', icon: bleData.chipCapable ? '✅' : '❌', label: chipLabel, value: bleData.chipCapable ? yesText : noText },
        { className: bleData.stackAvailable ? 'ok' : (bleData.chipCapable ? 'warn' : 'ko'), icon: bleData.stackAvailable ? '✅' : (bleData.chipCapable ? '⚠️' : '❌'), label: stackLabel, value: bleData.stackAvailable ? yesText : (bleData.chipCapable ? missingText : noText) },
        { className: bleData.enabled ? 'ok' : (bleData.stackAvailable ? 'warn' : 'ko'), icon: bleData.enabled ? '✅' : (bleData.stackAvailable ? '⚠️' : '❌'), label: runtimeLabel, value: bleData.enabled ? yesText : (bleData.stackAvailable ? missingText : noText) }
      ];
      entries.forEach(entry => {
        const li = document.createElement('li');
        const pill = document.createElement('span');
        pill.className = 'status-pill ' + entry.className;
        pill.textContent = entry.icon;
        const label = document.createElement('span');
        label.className = 'label';
        label.textContent = entry.label;
        const value = document.createElement('span');
        value.className = 'value';
        value.textContent = entry.value;
        li.appendChild(pill);
        li.appendChild(label);
        li.appendChild(value);
        bleDetails.appendChild(li);
      });
      const backendItem = document.createElement('li');
      backendItem.className = 'wireless-backend';
      const backendLabel = document.createElement('span');
      backendLabel.className = 'label';
      backendLabel.textContent = translations.ble_backend_label || 'Pile logicielle BLE';
      const backendValue = document.createElement('span');
      backendValue.className = 'value';
      backendValue.textContent = bleData.backend || (translations.ble_backend_missing || 'Pile BLE absente');
      backendItem.appendChild(backendLabel);
      backendItem.appendChild(backendValue);
      bleDetails.appendChild(backendItem);
    }

    const bleHint = document.getElementById('ble-summary-hint');
    if (bleHint) {
      const hint = bleData.hint || '';
      if (hint) {
        bleHint.style.display = 'block';
        bleHint.textContent = hint;
      } else {
        bleHint.style.display = 'none';
        bleHint.textContent = '';
      }
    }

    const bleButton = document.getElementById('ble-scan-button');
    const bleMonitor = document.getElementById('ble-status');
    const canScan = !!bleData.enabled;
    if (bleButton) {
      bleButton.disabled = !canScan;
      bleButton.title = (!canScan && (bleData.hint || bleData.status)) ? (bleData.hint || bleData.status) : '';
    }
    if (bleMonitor && bleMonitor.dataset.locked !== '1' && (bleMonitor.dataset.state !== 'results' || !canScan)) {
      if (canScan) {
        bleMonitor.textContent = translations.ble_click_to_scan || 'Cliquez pour scanner';
        bleMonitor.dataset.state = 'ready';
      } else {
        bleMonitor.textContent = bleData.status || (translations.ble_not_supported || 'Bluetooth LE non disponible');
        bleMonitor.dataset.state = 'unavailable';
      }
    }
  }
)JS";

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
  js += "if(tabName==='overview'){const r=await fetch('/api/overview');const d=await r.json();tab.innerHTML=buildOverview(d);applyOverviewData(d);updateRealtimeValues(d);}";
  js += "else if(tabName==='leds'){const r=await fetch('/api/leds-info');const d=await r.json();tab.innerHTML=buildLeds(d);}";
  js += "else if(tabName==='screens'){const r=await fetch('/api/screens-info');const d=await r.json();tab.innerHTML=buildScreens(d);}";
  js += "else if(tabName==='tests'){tab.innerHTML=buildTests();}";
  js += "else if(tabName==='gpio'){tab.innerHTML=buildGpio();}";
  js += "else if(tabName==='wifi'){tab.innerHTML=buildWifi();await loadWirelessStatus();}";
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

  js += "const psram=d.memory.psram||{};";
  js += "h+='<h3>PSRAM (Externe)</h3><div class=\"info-grid\">';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+(translations.status||'Statut')+'</div><div class=\"info-value\" id=\"psram-status\">'+(psram.status||'-')+'</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+(translations.psram_mode_label||'Mode supporté')+'</div><div class=\"info-value\" id=\"psram-mode\">'+(psram.mode_label||psram.mode||'-')+'</div></div>';";
  js += "if(psram.available&&psram.total>0){";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+(translations.total_size||'Total')+'</div><div class=\"info-value\" id=\"psram-total\">'+(psram.total/1048576).toFixed(2)+' MB</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+(translations.free||'Libre')+'</div><div class=\"info-value\" id=\"psram-free\">'+(psram.free/1048576).toFixed(2)+' MB</div></div>';";
  js += "h+='<div class=\"info-item\"><div class=\"info-label\">'+(translations.used||'Utilisée')+'</div><div class=\"info-value\" id=\"psram-used\">'+(psram.used/1048576).toFixed(2)+' MB</div></div>';";
  js += "}";
  js += "if(psram.hint){h+='<div class=\"info-item\" style=\"grid-column:1/-1\"><div class=\"info-label\">'+(translations.recommendation||'Recommandation')+'</div><div class=\"info-value\" id=\"psram-hint\">'+psram.hint+'</div></div>'; }";
  js += "h+='</div>';";
  js += "if(psram.available&&psram.total>0){const psramPct=((psram.used/psram.total)*100).toFixed(1);h+='<div class=\"progress-bar\"><div class=\"progress-fill\" id=\"psram-progress\" style=\"width:'+psramPct+'%\">'+psramPct+'%</div></div>'; }";
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

  // Build Wireless
  js += "function buildWifi(){";
  js += "let h='<div class=\"section\"><h2>📶 '+(translations.wireless_status||'Statut sans fil')+'</h2>';";
  js += "h+='<div class=\"wireless-summary\">';";
  js += "h+='<div class=\"card wireless-card\"><div class=\"wireless-card-title\">'+(translations.wifi_label||'Wi-Fi')+'</div>';";
  js += "h+='<div class=\"wireless-status\" id=\"wifi-summary-status\">'+(translations.wireless_loading||'...')+'</div>';";
  js += "h+='<ul class=\"wireless-list\" id=\"wifi-summary-details\"><li class=\"wireless-empty\">'+(translations.wireless_loading||'...')+'</li></ul></div>';";
  js += "h+='<div class=\"card wireless-card\"><div class=\"wireless-card-title\">'+(translations.ble_label||'Bluetooth Low Energy')+'</div>';";
  js += "h+='<div class=\"wireless-status\" id=\"ble-summary-status\">'+(translations.wireless_loading||'...')+'</div>';";
  js += "h+='<ul class=\"wireless-list\" id=\"ble-summary-details\"><li class=\"wireless-empty\">'+(translations.wireless_loading||'...')+'</li></ul>';";
  js += "h+='<div class=\"wireless-hint\" id=\"ble-summary-hint\" style=\"display:none\"></div></div>';";
  js += "h+='</div></div>';";
  js += "h+='<div class=\"section\"><h2>📡 '+(translations.wifi_scanner||'Scanner WiFi')+'</h2>';";
  js += "h+='<div style=\"text-align:center;margin:20px 0\"><button class=\"btn btn-primary\" onclick=\"scanWiFi()\">🔍 '+(translations.scan_networks||'Scanner les réseaux')+'</button>';";
  js += "h+='<div id=\"wifi-status\" class=\"status-live\">'+(translations.wifi_click_to_scan||'Cliquez pour scanner')+'</div></div>';";
  js += "h+='<div id=\"wifi-results\" class=\"wifi-list\"></div></div>';";
  js += "h+='<div class=\"section\"><h2>🛰️ '+(translations.ble_scanner||'Scanner Bluetooth Low Energy')+'</h2>';";
  js += "h+='<div style=\"text-align:center;margin:20px 0\"><button class=\"btn btn-primary\" id=\"ble-scan-button\" onclick=\"scanBLE()\">🔍 '+(translations.scan_ble_devices||'Scanner les appareils BLE')+'</button>';";
  js += "h+='<div id=\"ble-status\" class=\"status-live\">'+(translations.ble_click_to_scan||'Cliquez pour scanner')+'</div></div>';";
  js += "h+='<div id=\"ble-results\" class=\"ble-list\"></div></div>';";
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
  js += "const status=document.getElementById('wifi-status');";
  js += "const results=document.getElementById('wifi-results');";
  js += "if(status)status.textContent=translations.scanning||'Scan...';";
  js += "if(results)results.innerHTML='<div class=\\\"loading\\\"></div>';";
  js += "try{";
  js += "const r=await fetch('/api/wifi-scan');";
  js += "const d=await r.json();";
  js += "let h='';";
  js += "const channelLabel=translations.wifi_channel||'Canal WiFi';";
  js += "if(d.networks&&d.networks.length){";
  js += "d.networks.forEach(n=>{";
  js += "const icon=n.rssi>=-60?'🟢':n.rssi>=-70?'🟡':'🔴';";
  js += "const color=n.rssi>=-60?'#28a745':n.rssi>=-70?'#ffc107':'#dc3545';";
  js += "const name=n.ssid||translations.unknown||'(hidden)';";
  js += "h+='<div class=\"wifi-item\"><div style=\"display:flex;justify-content:space-between\"><div><strong>'+icon+' '+name+'</strong><br><small>'+n.bssid+' | '+channelLabel+' '+n.channel+'</small></div>';";
  js += "h+='<div style=\"font-size:1.3em;font-weight:bold;color:'+color+'\">'+n.rssi+' dBm</div></div></div>';";
  js += "});";
  js += "}else{";
  js += "h='<div class=\"wifi-item\">'+(translations.wifi_no_networks||'Aucun réseau détecté')+'</div>';";
  js += "}";
  js += "if(results)results.innerHTML=h;";
  js += "if(status)status.textContent=((d.networks&&d.networks.length)||0)+' '+(translations.networks||'réseaux');";
  js += "}catch(e){";
  js += "console.error('scanWiFi',e);";
  js += "if(status){status.textContent='❌ '+e;status.dataset.state='error';}";
  js += "if(results)results.innerHTML='';";
  js += "}finally{if(status){status.dataset.locked='0';}}";
  js += "}";

  js += "async function scanBLE(){";
  js += "const status=document.getElementById('ble-status');";
  js += "if(status){status.dataset.locked='1';}";
  js += "const results=document.getElementById('ble-results');";
  js += "if(status)status.textContent=translations.scanning||'Scan...';";
  js += "if(results)results.innerHTML='<div class=\\\"loading\\\"></div>';";
  js += "try{";
  js += "const r=await fetch('/api/ble-scan');";
  js += "const d=await r.json();";
  js += "if(!d.supported){";
  js += "if(results)results.innerHTML='';";
  js += "if(status){status.textContent=d.message||(translations.ble_not_supported||'Bluetooth LE non disponible');status.dataset.state='unavailable';}";
  js += "const btn=document.getElementById('ble-scan-button');";
  js += "if(btn){btn.disabled=true;btn.title=d.message||'';}";
  js += "return;";
  js += "}";
  js += "if(!d.devices||!d.devices.length){";
  js += "if(results)results.innerHTML='';";
  js += "if(status){status.textContent=translations.ble_no_devices||'Aucun appareil détecté';status.dataset.state='results';}";
  js += "return;";
  js += "}";
  js += "let h='';";
  js += "d.devices.forEach(dev=>{";
  js += "const icon=dev.rssi>=-60?'🟢':dev.rssi>=-70?'🟡':'🔴';";
  js += "const color=dev.rssi>=-60?'#28a745':dev.rssi>=-70?'#ffc107':'#dc3545';";
  js += "const name=dev.name||translations.unknown||'Inconnu';";
  js += "h+='<div class=\"ble-item\"><div style=\"display:flex;justify-content:space-between\"><div><strong>'+icon+' '+name+'</strong><br><small>'+dev.address+'</small></div>';";
  js += "h+='<div style=\"font-size:1.3em;font-weight:bold;color:'+color+'\">'+dev.rssi+' dBm</div></div></div>';";
  js += "});";
  js += "if(results)results.innerHTML=h;";
  js += "if(status){status.textContent=d.count+' '+(translations.devices||'périphérique(s)');status.dataset.state='results';}";
  js += "}catch(e){";
  js += "console.error('scanBLE',e);";
  js += "if(status){status.textContent='❌ '+e;status.dataset.state='error';}";
  js += "if(results)results.innerHTML='';";
  js += "}finally{if(status){status.dataset.locked='0';}}";
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
  js += "if(i){if(c){i.classList.remove('status-offline');i.classList.add('status-online');}";
  js += "else{i.classList.remove('status-online');i.classList.add('status-offline');}}";
  js += "const txt=document.getElementById('serviceStatus');";
  js += "if(txt){const online=(translations.online_status||'Connecté');const offline=(translations.offline_status||'Déconnecté');";
  js += "txt.textContent=c?online:offline;txt.classList.toggle('offline',!c);}";
  js += "}";

  js += "function updateRealtimeValues(data){";
  js += "if(!data)return;";
  js += "const chip=data.chip||{};";
  js += "const memory=data.memory||{};";
  js += "const wifi=data.wifi||{};";
  js += "const uptimeEl=document.getElementById('uptime');";
  js += "if(uptimeEl&&typeof chip.uptime!=='undefined')uptimeEl.textContent=formatUptime(chip.uptime);";
  js += "const summaryUptime=document.getElementById('summaryUptime');";
  js += "if(summaryUptime&&typeof chip.uptime!=='undefined')summaryUptime.textContent=formatUptime(chip.uptime);";
  js += "const tempEl=document.getElementById('temperature');";
  js += "if(tempEl&&typeof chip.temperature!=='undefined'&&chip.temperature!==-999){const tempVal=typeof chip.temperature==='number'?chip.temperature:parseFloat(chip.temperature);if(!isNaN(tempVal))tempEl.textContent=tempVal.toFixed(1)+' °C';}";
  js += "const ipEl=document.getElementById('ipAddress');";
  js += "if(ipEl)ipEl.textContent=(wifi.ip&&wifi.ip.length)?wifi.ip:'—';";
  js += "const summaryNetwork=document.getElementById('summaryNetwork');";
  js += "if(summaryNetwork)summaryNetwork.textContent=(wifi.ssid&&wifi.ssid.length)?wifi.ssid:(translations.wifi_not_connected||'Non connecté');";
  js += "const qualityEl=document.getElementById('networkQuality');";
  js += "if(qualityEl)qualityEl.textContent=wifi.quality||'—';";
  js += "const sram=memory.sram||{};";
  js += "const sramFree=document.getElementById('sram-free');";
  js += "if(sramFree&&typeof sram.free!=='undefined')sramFree.textContent=(sram.free/1024).toFixed(2)+' KB';";
  js += "const sramUsed=document.getElementById('sram-used');";
  js += "if(sramUsed&&typeof sram.used!=='undefined')sramUsed.textContent=(sram.used/1024).toFixed(2)+' KB';";
  js += "const sramProgress=document.getElementById('sram-progress');";
  js += "if(sramProgress&&typeof sram.total!=='undefined'&&sram.total>0){const pct=((sram.used/sram.total)*100).toFixed(1);sramProgress.style.width=pct+'%';sramProgress.textContent=pct+'%';}";
  js += "const psram=memory.psram||{};";
  js += "if(psram&&typeof psram.total!=='undefined'&&psram.total>0){";
  js += "const psramFree=document.getElementById('psram-free');if(psramFree)psramFree.textContent=(psram.free/1048576).toFixed(2)+' MB';";
  js += "const psramUsed=document.getElementById('psram-used');if(psramUsed)psramUsed.textContent=(psram.used/1048576).toFixed(2)+' MB';";
  js += "const psramProgress=document.getElementById('psram-progress');";
  js += "if(psramProgress){const pct=((psram.used/psram.total)*100).toFixed(1);psramProgress.style.width=pct+'%';psramProgress.textContent=pct+'%';}";
  js += "}";
  js += "const fragEl=document.getElementById('fragmentation');";
  js += "if(fragEl&&typeof memory.fragmentation!=='undefined')fragEl.textContent=parseFloat(memory.fragmentation).toFixed(1)+'%';";
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
  js += "updateLiveData();";
  js += "const msg=(translations.language_changed||'Langue changée')+' : '+lang.toUpperCase();";
  js += "showUpdateIndicator(msg);hideUpdateIndicator(2000);";
  js += "}).catch(()=>{showUpdateIndicator('Erreur changement langue');hideUpdateIndicator(2500);});";
  js += "}";

  js += "function updateInterfaceTexts(t){";
  js += "const tabs=['overview','leds','screens','tests','gpio','wifi','benchmark','export'];";
  js += "const btns=document.querySelectorAll('.nav-btn');";
  js += "btns.forEach((btn,i)=>{if(t[tabs[i]])btn.textContent=t[tabs[i]];});";
  js += "document.querySelectorAll('[data-i18n]').forEach(el=>{const key=el.dataset.i18n;if(key&&t[key])el.textContent=t[key];});";
  js += "const gpioWarn=document.getElementById('gpio-warning');if(gpioWarn&&t.gpio_fail_hint)gpioWarn.textContent=t.gpio_fail_hint;";
  js += "const oledInput=document.getElementById('oledText');if(oledInput&&t.custom_message)oledInput.placeholder=t.custom_message;";
  js += "if(typeof isConnected!=='undefined'){updateStatusIndicator(isConnected);}";
  js += "}";

  return js;
}

#endif // WEB_INTERFACE_H
