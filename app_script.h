#ifndef APP_SCRIPT_H
#define APP_SCRIPT_H

#include <Arduino.h>
#include "languages.h"

inline String escapeForJS(const String& input) {
  String output;
  output.reserve(input.length() * 2 + 4);
  for (size_t i = 0; i < input.length(); ++i) {
    char c = input.charAt(i);
    switch (c) {
      case '\\':
        output += "\\\\";
        break;
      case '"':
        output += "\\\"";
        break;
      case '\'':
        output += "\\'";
        break;
      case '\n':
        output += "\\n";
        break;
      case '\r':
        output += "\\r";
        break;
      case '\t':
        output += "\\t";
        break;
      default:
        output += c;
        break;
    }
  }
  return output;
}

inline String buildAppScript() {
  static const char APP_SCRIPT_TEMPLATE[] PROGMEM = R"JS((() => {
  let currentLang = "{{LANG}}";
  const fallbackChar = "{{FALLBACK}}";
  const labels = {
    connected: "{{CONNECTED}}",
    disconnected: "{{DISCONNECTED}}",
    channelPrefix: "{{CHANNEL_PREFIX}}"
  };
  const messages = {
    testing: "{{TESTING}}",
    testingOled: "{{TESTING_OLED}}",
    reconfiguring: "{{RECONFIGURING}}",
    configUpdated: "{{CONFIG_UPDATED}}",
    configError: "{{CONFIG_ERROR}}",
    errorPrefix: "{{ERROR_PREFIX}}",
    scanning: "{{SCANNING}}",
    ok: "{{OK}}",
    fail: "{{FAIL}}",
    completed: "{{COMPLETED}}",
    gpio: "{{GPIO}}",
    tested: "{{TESTED}}",
    unknown: "{{UNKNOWN}}",
    networks: "{{NETWORKS}}",
    devices: "{{DEVICES}}"
  };

  const indicator = {
    wifiLabel: "{{WIFI_LABEL}}",
    bluetoothLabel: "{{BT_LABEL}}",
    offline: "{{OFFLINE}}",
    unavailable: "{{UNAVAILABLE}}"
  };

  let lastBuiltinGPIO = null;
  let lastNeoGPIO = null;
  let lastNeoCount = null;
  let lastOledConfig = { sda: null, scl: null };

  const setHTML = (id, text) => {
    const el = document.getElementById(id);
    if (el) {
      el.innerHTML = text;
    }
  };

  const setText = (id, text) => {
    const el = document.getElementById(id);
    if (el) {
      el.textContent = text;
    }
  };

  const updateText = (id, value, fallback) => {
    const el = document.getElementById(id);
    if (!el) return;
    const resolved = value === undefined || value === null || value === '' ? (fallback || fallbackChar) : value;
    el.textContent = resolved;
  };

  const markActiveLanguage = lang => {
    document.querySelectorAll('.lang-btn[data-lang]').forEach(btn => {
      btn.classList.toggle('active', btn.dataset.lang === lang);
    });
    document.documentElement.setAttribute('lang', lang);
  };

  function changeLang(lang, evt) {
    if (evt && evt.preventDefault) {
      evt.preventDefault();
    }
    if (!lang || lang === currentLang) {
      return false;
    }
    fetch('/api/set-language?lang=' + lang)
      .then(r => r.json())
      .then(d => {
        if (!d.success) {
          throw new Error('Lang switch refused');
        }
        currentLang = lang;
        markActiveLanguage(lang);
        window.location.reload();
      })
      .catch(err => console.error('Lang switch error', err));
    return false;
  }

  function showTab(tabId, evt) {
    if (evt && evt.preventDefault) {
      evt.preventDefault();
    }
    if (!tabId) {
      return false;
    }
    document.querySelectorAll('.tab-content').forEach(el => {
      el.classList.toggle('active', el.id === tabId);
    });
    document.querySelectorAll('.nav-btn[data-nav]').forEach(btn => {
      btn.classList.toggle('active', btn.dataset.nav === tabId);
    });
    if (tabId === 'wireless') {
      refreshWirelessInfo();
    }
    return false;
  }

  function updateDotState(dotEl, state) {
    if (!dotEl) return;
    dotEl.classList.remove('online', 'offline', 'pending');
    dotEl.classList.add(state);
  }

  function applyBuiltinConfig(manual = true) {
    const gpioField = document.getElementById('ledGPIO');
    if (!gpioField) {
      return Promise.resolve();
    }
    const value = (gpioField.value || '').trim();
    if (!value.length) {
      return Promise.resolve();
    }
    if (!manual && value === lastBuiltinGPIO) {
      return Promise.resolve();
    }
    if (manual) {
      setText('builtin-led-status', messages.reconfiguring);
    }
    return fetch('/api/builtin-led-config?gpio=' + encodeURIComponent(value))
      .then(r => r.json())
      .then(d => {
        if (d && d.success) {
          lastBuiltinGPIO = value;
          if (d.message) {
            setHTML('builtin-led-status', d.message);
          } else if (manual) {
            setText('builtin-led-status', messages.configUpdated);
          }
          return true;
        }
        const errMessage = d && d.message ? d.message : messages.configError;
        if (manual) {
          setHTML('builtin-led-status', '❌ ' + errMessage);
        }
        throw new Error(errMessage);
      });
  }

  function ensureBuiltinConfigured() {
    const gpioField = document.getElementById('ledGPIO');
    if (!gpioField) {
      return Promise.resolve();
    }
    const value = (gpioField.value || '').trim();
    if (!value.length || value === lastBuiltinGPIO) {
      return Promise.resolve();
    }
    setText('builtin-led-status', messages.reconfiguring);
    return applyBuiltinConfig(false).catch(err => {
      setHTML('builtin-led-status', '❌ ' + (err && err.message ? err.message : messages.configError));
      throw err;
    });
  }

  function applyNeoPixelConfig(manual = true) {
    const gpioField = document.getElementById('neoGPIO');
    const countField = document.getElementById('neoCount');
    if (!gpioField || !countField) {
      return Promise.resolve();
    }
    const gpioValue = (gpioField.value || '').trim();
    const countValue = (countField.value || '').trim();
    if (!gpioValue.length || !countValue.length) {
      return Promise.resolve();
    }
    if (!manual && gpioValue === lastNeoGPIO && countValue === lastNeoCount) {
      return Promise.resolve();
    }
    if (manual) {
      setText('neopixel-status', messages.reconfiguring);
    }
    const url = `/api/neopixel-config?gpio=${encodeURIComponent(gpioValue)}&count=${encodeURIComponent(countValue)}`;
    return fetch(url)
      .then(r => r.json())
      .then(d => {
        if (d && d.success) {
          lastNeoGPIO = gpioValue;
          lastNeoCount = countValue;
          if (d.message) {
            setHTML('neopixel-status', d.message);
          } else if (manual) {
            setText('neopixel-status', messages.configUpdated);
          }
          return true;
        }
        const msg = d && d.message ? d.message : messages.configError;
        if (manual) {
          setHTML('neopixel-status', '❌ ' + msg);
        }
        throw new Error(msg);
      });
  }

  function ensureNeoPixelConfigured() {
    const gpioField = document.getElementById('neoGPIO');
    const countField = document.getElementById('neoCount');
    if (!gpioField || !countField) {
      return Promise.resolve();
    }
    const gpioValue = (gpioField.value || '').trim();
    const countValue = (countField.value || '').trim();
    if (!gpioValue.length || !countValue.length) {
      return Promise.resolve();
    }
    if (gpioValue === lastNeoGPIO && countValue === lastNeoCount) {
      return Promise.resolve();
    }
    setText('neopixel-status', messages.reconfiguring);
    return applyNeoPixelConfig(false).catch(err => {
      setHTML('neopixel-status', '❌ ' + (err && err.message ? err.message : messages.configError));
      throw err;
    });
  }

  function applyOLEDConfig(manual = true) {
    const status = document.getElementById('oled-status');
    const sdaEl = document.getElementById('oledSDA');
    const sclEl = document.getElementById('oledSCL');
    if (!sdaEl || !sclEl) {
      return Promise.resolve();
    }
    const sda = (sdaEl.value || '').trim();
    const scl = (sclEl.value || '').trim();
    if (!sda.length || !scl.length) {
      return Promise.resolve();
    }
    if (!manual && lastOledConfig.sda === sda && lastOledConfig.scl === scl) {
      return Promise.resolve();
    }
    if (status && manual) {
      status.textContent = messages.reconfiguring;
    }
    const url = `/api/oled-config?sda=${encodeURIComponent(sda)}&scl=${encodeURIComponent(scl)}`;
    return fetch(url)
      .then(r => r.json())
      .then(d => {
        if (d && d.success) {
          lastOledConfig = { sda, scl };
          if (status) {
            const msg = d.message || messages.configUpdated;
            status.innerHTML = '✅ ' + msg;
          }
          const pins = document.getElementById('oled-pins');
          if (pins) {
            pins.textContent = `SDA:${sda} SCL:${scl}`;
          }
          return true;
        }
        const msg = d && d.message ? d.message : messages.configError;
        if (status) {
          status.innerHTML = '❌ ' + msg;
        }
        throw new Error(msg);
      });
  }

  function ensureOLEDConfigured() {
    const sdaEl = document.getElementById('oledSDA');
    const sclEl = document.getElementById('oledSCL');
    const status = document.getElementById('oled-status');
    if (!sdaEl || !sclEl) {
      return Promise.resolve();
    }
    const sda = (sdaEl.value || '').trim();
    const scl = (sclEl.value || '').trim();
    if (!sda.length || !scl.length) {
      return Promise.resolve();
    }
    if (lastOledConfig.sda === sda && lastOledConfig.scl === scl) {
      return Promise.resolve();
    }
    if (status) {
      status.textContent = messages.reconfiguring;
    }
    return applyOLEDConfig(false).catch(err => {
      if (status) {
        status.innerHTML = '❌ ' + (err && err.message ? err.message : messages.configError);
      }
      throw err;
    });
  }

  function configBuiltinLED() {
    applyBuiltinConfig(true).catch(err => {
      console.error('LED config', err);
      setHTML('builtin-led-status', '❌ ' + (err && err.message ? err.message : messages.configError));
    });
  }

  function testBuiltinLED() {
    ensureBuiltinConfigured()
      .then(() => {
        setText('builtin-led-status', messages.testing);
        return fetch('/api/builtin-led-test');
      })
      .then(r => r.json())
      .then(d => setHTML('builtin-led-status', d.result || ''))
      .catch(err => {
        console.error('LED test', err);
        setHTML('builtin-led-status', '❌ ' + (err && err.message ? err.message : messages.configError));
      });
  }

  function ledBlink() {
    ensureBuiltinConfigured()
      .then(() => fetch('/api/builtin-led-control?action=blink'))
      .then(r => r.json())
      .then(d => setHTML('builtin-led-status', d.message || ''))
      .catch(err => {
        console.error('LED blink', err);
        setHTML('builtin-led-status', '❌ ' + (err && err.message ? err.message : messages.configError));
      });
  }

  function ledFade() {
    ensureBuiltinConfigured()
      .then(() => fetch('/api/builtin-led-control?action=fade'))
      .then(r => r.json())
      .then(d => setHTML('builtin-led-status', d.message || ''))
      .catch(err => {
        console.error('LED fade', err);
        setHTML('builtin-led-status', '❌ ' + (err && err.message ? err.message : messages.configError));
      });
  }

  function ledOff() {
    ensureBuiltinConfigured()
      .then(() => fetch('/api/builtin-led-control?action=off'))
      .then(r => r.json())
      .then(d => setHTML('builtin-led-status', d.message || ''))
      .catch(err => {
        console.error('LED off', err);
        setHTML('builtin-led-status', '❌ ' + (err && err.message ? err.message : messages.configError));
      });
  }

  function configNeoPixel() {
    applyNeoPixelConfig(true).catch(err => {
      console.error('NeoPixel config', err);
      setHTML('neopixel-status', '❌ ' + (err && err.message ? err.message : messages.configError));
    });
  }

  function testNeoPixel() {
    ensureNeoPixelConfigured()
      .then(() => {
        setText('neopixel-status', messages.testing);
        return fetch('/api/neopixel-test');
      })
      .then(r => r.json())
      .then(d => setHTML('neopixel-status', d.result || ''))
      .catch(err => {
        console.error('NeoPixel test', err);
        setHTML('neopixel-status', '❌ ' + (err && err.message ? err.message : messages.configError));
      });
  }

  function neoPattern(pattern) {
    ensureNeoPixelConfigured()
      .then(() => fetch('/api/neopixel-pattern?pattern=' + encodeURIComponent(pattern)))
      .then(r => r.json())
      .then(d => setHTML('neopixel-status', d.message || ''))
      .catch(err => {
        console.error('NeoPixel pattern', err);
        setHTML('neopixel-status', '❌ ' + (err && err.message ? err.message : messages.configError));
      });
  }

  function neoCustomColor() {
    const picker = document.getElementById('neoColor');
    if (!picker) return;
    const value = picker.value || '#000000';
    const r = parseInt(value.substring(1, 3), 16) || 0;
    const g = parseInt(value.substring(3, 5), 16) || 0;
    const b = parseInt(value.substring(5, 7), 16) || 0;
    ensureNeoPixelConfigured()
      .then(() => fetch(`/api/neopixel-color?r=${r}&g=${g}&b=${b}`))
      .then(r => r.json())
      .then(d => setHTML('neopixel-status', d.message || ''))
      .catch(err => {
        console.error('NeoPixel color', err);
        setHTML('neopixel-status', '❌ ' + (err && err.message ? err.message : messages.configError));
      });
  }

  function testOLED() {
    ensureOLEDConfigured()
      .then(() => {
        setText('oled-status', messages.testingOled);
        return fetch('/api/oled-test');
      })
      .then(r => r.json())
      .then(d => setHTML('oled-status', d.result || ''))
      .catch(err => {
        console.error('OLED test', err);
        setHTML('oled-status', '❌ ' + (err && err.message ? err.message : messages.configError));
      });
  }

  function oledStep(step) {
    ensureOLEDConfigured()
      .then(() => {
        setText('oled-status', messages.testing);
        return fetch('/api/oled-step?step=' + encodeURIComponent(step));
      })
      .then(r => r.json())
      .then(d => {
        if (!d) return;
        if (d.success) {
          setHTML('oled-status', d.message || '');
        } else {
          setHTML('oled-status', '❌ ' + (d.message || messages.configError));
        }
      })
      .catch(err => setHTML('oled-status', '❌ ' + (err && err.message ? err.message : err)));
  }

  function oledMessage() {
    const input = document.getElementById('oledMsg');
    if (!input) return;
    ensureOLEDConfigured()
      .then(() => fetch('/api/oled-message?message=' + encodeURIComponent(input.value || '')))
      .then(r => r.json())
      .then(d => setHTML('oled-status', d.message || ''))
      .catch(err => {
        console.error('OLED message', err);
        setHTML('oled-status', '❌ ' + (err && err.message ? err.message : messages.configError));
      });
  }

  function configOLED() {
    applyOLEDConfig(true).catch(err => {
      console.error('OLED config', err);
      const status = document.getElementById('oled-status');
      if (status) {
        status.innerHTML = '❌ ' + messages.errorPrefix + (err && err.message ? err.message : messages.configError);
      }
    });
  }

  function testADC() {
    setText('adc-status', messages.testing);
    fetch('/api/adc-test')
      .then(r => r.json())
      .then(data => {
        if (!data) return;
        let html = '';
        if (Array.isArray(data.readings)) {
          data.readings.forEach(a => {
            const voltage = a.voltage !== undefined && a.voltage !== null ? Number(a.voltage).toFixed(2) : '0.00';
            html += `<div class="info-item"><div class="info-label">GPIO ${a.pin}</div><div class="info-value">${a.raw} (${voltage}V)</div></div>`;
          });
        }
        setHTML('adc-results', html);
        setHTML('adc-status', data.result || '');
      })
      .catch(err => console.error('ADC test', err));
  }

  function testTouch() {
    setText('touch-status', messages.testing);
    fetch('/api/touch-test')
      .then(r => r.json())
      .then(data => {
        if (!data) return;
        let html = '';
        if (Array.isArray(data.readings)) {
          data.readings.forEach(t => {
            html += `<div class="info-item"><div class="info-label">Touch${t.pin}</div><div class="info-value">${t.value}</div></div>`;
          });
        }
        setHTML('touch-results', html);
        setHTML('touch-status', data.result || '');
      })
      .catch(err => console.error('Touch test', err));
  }

  function testPWM() {
    setText('pwm-status', messages.testing);
    fetch('/api/pwm-test')
      .then(r => r.json())
      .then(d => setHTML('pwm-status', d.result || ''))
      .catch(err => console.error('PWM test', err));
  }

  function scanSPI() {
    setText('spi-status', messages.scanning);
    fetch('/api/spi-scan')
      .then(r => r.json())
      .then(d => setHTML('spi-status', d.info || ''))
      .catch(err => console.error('SPI scan', err));
  }

  function listPartitions() {
    setText('partitions-results', messages.scanning);
    fetch('/api/partitions-list')
      .then(r => r.json())
      .then(d => setHTML('partitions-results', d.partitions || ''))
      .catch(err => console.error('Partitions list', err));
  }

  function stressTest() {
    setText('stress-status', messages.testing);
    fetch('/api/stress-test')
      .then(r => r.json())
      .then(d => setHTML('stress-status', d.result || ''))
      .catch(err => console.error('Stress test', err));
  }

  function testAllGPIO() {
    setText('gpio-status', messages.testing);
    fetch('/api/test-gpio')
      .then(r => r.json())
      .then(data => {
        if (!data) return;
        let html = '';
        if (Array.isArray(data.results)) {
          data.results.forEach(g => {
            const status = g.working ? `✅ ${messages.ok}` : `❌ ${messages.fail}`;
            html += `<div class="gpio-item ${g.working ? 'gpio-ok' : 'gpio-fail'}">GPIO ${g.pin}<br>${status}</div>`;
          });
        }
        setHTML('gpio-results', html);
        const total = Array.isArray(data.results) ? data.results.length : 0;
        setText('gpio-status', `✅ ${messages.completed} - ${total} ${messages.gpio} ${messages.tested}`);
      })
      .catch(err => console.error('GPIO test', err));
  }

  function refreshWirelessInfo() {
    fetch('/api/wireless-info')
      .then(r => r.json())
      .then(data => updateWirelessInfo(data))
      .catch(err => console.error('Wireless info', err));
  }

  function updateWirelessInfo(data) {
    if (!data) return;
    const wifi = data.wifi || {};
    const bt = data.bluetooth || {};
    const wifiDot = document.getElementById('wifi-status-dot');
    const wifiLabel = document.getElementById('wifi-status-label');
    const wifiConnected = !!wifi.connected;
    const wifiStateText = wifiConnected ? labels.connected : indicator.offline;
    if (wifiLabel) {
      wifiLabel.textContent = `${indicator.wifiLabel} · ${wifiStateText}`;
    }
    updateDotState(wifiDot, wifiConnected ? 'online' : 'offline');
    updateText('wifi-connection-state', wifi.connected ? `✅ ${labels.connected}` : `❌ ${labels.disconnected}`);
    updateText('wifi-ssid', wifi.ssid && wifi.ssid.length ? wifi.ssid : fallbackChar);
    const channel = typeof wifi.channel === 'number' && wifi.channel > 0 ? labels.channelPrefix + wifi.channel : fallbackChar;
    updateText('wifi-channel', channel);
    updateText('wifi-mode', wifi.mode || fallbackChar);
    updateText('wifi-sleep', wifi.sleep || fallbackChar);
    updateText('wifi-band', wifi.band || fallbackChar);
    updateText('wifi-band-mode', wifi.band_mode || fallbackChar);
    const txCode = typeof wifi.tx_power_code === 'number' ? wifi.tx_power_code : 'n/a';
    const txPower = typeof wifi.tx_power_dbm === 'number' ? `${Number(wifi.tx_power_dbm).toFixed(1)} dBm (${txCode})` : fallbackChar;
    updateText('wifi-tx-power', txPower);
    updateText('wifi-hostname', wifi.hostname || fallbackChar);
    if (wifi.connected) {
      updateText('wifi-rssi', `${wifi.rssi} dBm`);
      updateText('wifi-quality', wifi.quality || fallbackChar);
      updateText('wifi-ip', wifi.ip || fallbackChar);
      updateText('wifi-subnet', wifi.subnet || fallbackChar);
      updateText('wifi-gateway', wifi.gateway || fallbackChar);
      updateText('wifi-dns', wifi.dns || fallbackChar);
    } else {
      updateText('wifi-rssi', fallbackChar);
      updateText('wifi-quality', fallbackChar);
      updateText('wifi-ip', fallbackChar);
      updateText('wifi-subnet', fallbackChar);
      updateText('wifi-gateway', fallbackChar);
      updateText('wifi-dns', fallbackChar);
    }
    const capabilities = [];
    if (bt.classic) {
      capabilities.push(currentLang === 'fr' ? 'Classique' : 'Classic');
    }
    if (bt.ble) {
      capabilities.push('BLE');
    }
    updateText('bluetooth-capabilities', capabilities.length ? capabilities.join(', ') : fallbackChar);
    updateText('bluetooth-controller', bt.controller || fallbackChar);
    updateText('bluetooth-last-test', bt.last_test_message || fallbackChar);
    if (bt.last_test_message) {
      setText('bluetooth-status', bt.last_test_message);
    }
    const hint = bt.hint && bt.hint.length ? bt.hint : '\u00A0';
    setText('bluetooth-hint', hint);
    const btDot = document.getElementById('bt-status-dot');
    const btLabel = document.getElementById('bt-status-label');
    const hasBluetoothHardware = !!(bt.classic || bt.ble);
    let btStateClass = 'pending';
    let btStateText = indicator.unavailable;
    if (hasBluetoothHardware) {
      if (!bt.compile_enabled) {
        btStateClass = 'offline';
        btStateText = indicator.unavailable;
      } else if (bt.controller_enabled || bt.last_test_success) {
        btStateClass = 'online';
        btStateText = bt.last_test_message || labels.connected;
      } else if (bt.controller_initialized) {
        btStateClass = 'pending';
        btStateText = bt.controller || indicator.offline;
      } else {
        btStateClass = 'offline';
        btStateText = bt.controller || indicator.offline;
      }
    }
    if (bt.last_test_message) {
      btStateText = bt.last_test_message;
      btStateClass = bt.last_test_success ? 'online' : btStateClass;
    }
    if (!hasBluetoothHardware) {
      btStateClass = 'pending';
      btStateText = indicator.unavailable;
    }
    if (btLabel) {
      btLabel.textContent = `${indicator.bluetoothLabel} · ${btStateText}`;
    }
    updateDotState(btDot, btStateClass);
  }

  function testBluetooth() {
    setText('bluetooth-status', messages.testing);
    fetch('/api/bluetooth-test')
      .then(r => r.json())
      .then(d => {
        if (!d) return;
        setText('bluetooth-status', d.message || messages.unknown);
        refreshWirelessInfo();
      })
      .catch(err => setText('bluetooth-status', '❌ ' + err));
  }

  function scanWiFi() {
    setText('wifi-status', messages.scanning);
    fetch('/api/wifi-scan')
      .then(r => r.json())
      .then(data => {
        if (!data) return;
        let html = '';
        if (Array.isArray(data.networks)) {
          data.networks.forEach(n => {
            const strength = n.rssi >= -60 ? '🟢' : (n.rssi >= -70 ? '🟡' : '🔴');
            html += `<div class="wifi-item"><div style="display:flex;justify-content:space-between"><div><strong>${strength} ${n.ssid}</strong><br><small>${n.bssid} | Ch${n.channel} | ${n.encryption}</small></div><div style="font-size:1.2em;font-weight:bold">${n.rssi} dBm</div></div></div>`;
          });
        }
        setHTML('wifi-results', html);
        const total = Array.isArray(data.networks) ? data.networks.length : 0;
        setText('wifi-status', `${total} ${messages.networks}`);
      })
      .catch(err => console.error('WiFi scan', err));
  }

  function scanI2C() {
    fetch('/api/i2c-scan')
      .then(r => r.json())
      .then(d => {
        if (!d) return;
        const count = document.getElementById('i2c-count');
        if (count) {
          count.innerHTML = `${d.count} ${messages.devices}`;
        }
        const devices = document.getElementById('i2c-devices');
        if (devices) {
          devices.innerHTML = d.devices && d.devices.length ? d.devices : fallbackChar;
        }
        console.log(`I2C: ${d.count} périphérique(s)\n${d.devices}`);
      })
      .catch(err => console.error('I2C scan', err));
  }

  function runBenchmarks() {
    setText('cpu-bench', messages.testing);
    setText('mem-bench', messages.testing);
    fetch('/api/benchmark')
      .then(r => r.json())
      .then(data => {
        if (!data) return;
        setText('cpu-bench', `${data.cpu} us`);
        setText('mem-bench', `${data.memory} us`);
        if (data.cpuPerf !== undefined) {
          setText('cpu-perf', Number(data.cpuPerf).toFixed(2) + ' MFLOPS');
        }
        if (data.memSpeed !== undefined) {
          setText('mem-speed', Number(data.memSpeed).toFixed(2) + ' MB/s');
        }
      })
      .catch(err => console.error('Benchmarks', err));
  }

  document.addEventListener('DOMContentLoaded', () => {
    markActiveLanguage(currentLang);
    document.querySelectorAll('.nav-btn[data-nav]').forEach(btn => {
      btn.addEventListener('click', evt => showTab(btn.dataset.nav, evt));
    });
    document.querySelectorAll('.lang-btn[data-lang]').forEach(btn => {
      btn.addEventListener('click', evt => changeLang(btn.dataset.lang, evt));
    });
    const activeTab = document.querySelector('.tab-content.active');
    if (activeTab) {
      showTab(activeTab.id);
    }
    refreshWirelessInfo();
    scanI2C();
  });

  const api = {
    changeLang,
    showTab,
    configBuiltinLED,
    testBuiltinLED,
    ledBlink,
    ledFade,
    ledOff,
    configNeoPixel,
    testNeoPixel,
    neoPattern,
    neoCustomColor,
    testOLED,
    oledStep,
    oledMessage,
    configOLED,
    testADC,
    testTouch,
    testPWM,
    scanSPI,
    listPartitions,
    stressTest,
    testAllGPIO,
    refreshWirelessInfo,
    scanWiFi,
    testBluetooth,
    scanI2C,
    runBenchmarks
  };

  Object.entries(api).forEach(([name, fn]) => {
    window[name] = fn;
  });
})();
)JS";

  String script = String(FPSTR(APP_SCRIPT_TEMPLATE));
  script.replace(F("{{LANG}}"), currentLanguage == LANG_FR ? F("fr") : F("en"));
  String fallbackValue = String("—");
  script.replace(F("{{FALLBACK}}"), escapeForJS(fallbackValue));
  String connectedLabel = String(T().connected);
  script.replace(F("{{CONNECTED}}"), escapeForJS(connectedLabel));
  String disconnectedLabel = String(T().disconnected);
  script.replace(F("{{DISCONNECTED}}"), escapeForJS(disconnectedLabel));
  script.replace(F("{{WIFI_LABEL}}"), escapeForJS(String(T().indicator_wifi)));
  script.replace(F("{{BT_LABEL}}"), escapeForJS(String(T().indicator_bluetooth)));
  script.replace(F("{{OFFLINE}}"), escapeForJS(String(T().disconnected)));
  script.replace(F("{{UNAVAILABLE}}"), escapeForJS(String(T().indicator_unavailable)));
  String channelPrefix = (currentLanguage == LANG_FR) ? String("Canal ") : String("Ch ");
  script.replace(F("{{CHANNEL_PREFIX}}"), escapeForJS(channelPrefix));
  script.replace(F("{{TESTING}}"), escapeForJS(String(T().testing)));
  script.replace(F("{{TESTING_OLED}}"), escapeForJS(String(T().testing) + String(" (25s)...")));
  script.replace(F("{{RECONFIGURING}}"), escapeForJS(String(T().reconfiguring)));
  script.replace(F("{{CONFIG_UPDATED}}"), escapeForJS(String(T().config_updated)));
  script.replace(F("{{CONFIG_ERROR}}"), escapeForJS(String(T().config_error)));
  script.replace(F("{{ERROR_PREFIX}}"), escapeForJS(String(T().error_prefix)));
  script.replace(F("{{SCANNING}}"), escapeForJS(String(T().scanning)));
  script.replace(F("{{OK}}"), escapeForJS(String(T().ok)));
  script.replace(F("{{FAIL}}"), escapeForJS(String(T().fail)));
  script.replace(F("{{COMPLETED}}"), escapeForJS(String(T().completed)));
  script.replace(F("{{GPIO}}"), escapeForJS(String(T().gpio)));
  script.replace(F("{{TESTED}}"), escapeForJS(String(T().tested)));
  script.replace(F("{{UNKNOWN}}"), escapeForJS(String(T().unknown)));
  script.replace(F("{{NETWORKS}}"), escapeForJS(String(T().networks)));
  script.replace(F("{{DEVICES}}"), escapeForJS(String(T().devices)));

  return script;
}

#endif  // APP_SCRIPT_H
