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
    testingLed: "{{TESTING_LED}}",
    testingLedEffect: "{{TESTING_LED_EFFECT}}",
    testingLedFade: "{{TESTING_LED_FADE}}",
    testingLedOff: "{{TESTING_LED_OFF}}",
    testingNeopixel: "{{TESTING_NEOPIXEL}}",
    testingNeopixelEffect: "{{TESTING_NEOPIXEL_EFFECT}}",
    testingNeopixelColor: "{{TESTING_NEOPIXEL_COLOR}}",
    testingNeopixelOff: "{{TESTING_NEOPIXEL_OFF}}",
    testingOledStep: "{{TESTING_OLED_STEP}}",
    testingOledMessage: "{{TESTING_OLED_MESSAGE}}",
    testingAdc: "{{TESTING_ADC}}",
    testingTouch: "{{TESTING_TOUCH}}",
    testingPwm: "{{TESTING_PWM}}",
    testingSpi: "{{TESTING_SPI}}",
    testingPartitions: "{{TESTING_PARTITIONS}}",
    testingStress: "{{TESTING_STRESS}}",
    testingGpio: "{{TESTING_GPIO}}",
    testingBluetooth: "{{TESTING_BLUETOOTH}}",
    testingBenchmarks: "{{TESTING_BENCHMARKS}}",
    wifiWaiting: "{{WIFI_WAITING}}",
    scanning: "{{SCANNING}}",
    ok: "{{OK}}",
    fail: "{{FAIL}}",
    completed: "{{COMPLETED}}",
    gpio: "{{GPIO}}",
    tested: "{{TESTED}}",
    unknown: "{{UNKNOWN}}",
    networks: "{{NETWORKS}}",
    devices: "{{DEVICES}}",
    notTested: "{{NOT_TESTED}}"
  };

  const indicator = {
    wifiLabel: "{{WIFI_LABEL}}",
    bluetoothLabel: "{{BT_LABEL}}",
    offline: "{{OFFLINE}}",
    unavailable: "{{UNAVAILABLE}}",
    ap: "{{AP_STATE}}"
  };

  const toBool = value => {
    if (typeof value === 'boolean') {
      return value;
    }
    if (typeof value === 'string') {
      const lowered = value.toLowerCase();
      if (lowered === 'true') return true;
      if (lowered === 'false') return false;
    }
    if (typeof value === 'number') {
      return value !== 0;
    }
    if (value === null || value === undefined) {
      return false;
    }
    return !!value;
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

  const updateLedSummary = text => setText('led-summary', text);
  const updateNeoSummary = text => setText('neopixel-summary', text);
  const updateOledSummary = text => setText('oled-summary', text);

  function showStatus(targetId, text) {
    const el = document.getElementById(targetId);
    if (el) {
      el.textContent = typeof text === 'string' ? text : '';
    }
    console.debug('[UI]', targetId, text);
  }

  function clearStatus(targetId) {
    const el = document.getElementById(targetId);
    if (el) {
      el.textContent = '';
    }
  }

  const setStatusRunning = (id, label) => {
    showStatus(id, label || messages.testing);
  };

  const setStatusSuccess = (id, detail) => {
    const message = detail && detail.length ? detail : messages.completed;
    showStatus(id, message);
  };

  const setStatusError = (id, detail) => {
    const prefix = messages.errorPrefix || '';
    const raw = detail && detail.length ? detail : messages.configError;
    const text = raw.startsWith(prefix) ? raw : `${prefix}${raw}`;
    showStatus(id, text);
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
    const validStates = ['on', 'mid', 'off', 'blink'];
    const resolved = validStates.includes(state) ? state : 'off';
    dotEl.classList.remove('dot-on', 'dot-mid', 'dot-off', 'dot-blink');
    dotEl.classList.add('dot-' + resolved);
  }

  function updatePillState(pillEl, state) {
    if (!pillEl) return;
    const validStates = ['on', 'mid', 'off', 'blink'];
    const resolved = validStates.includes(state) ? state : 'off';
    pillEl.classList.remove('state-on', 'state-mid', 'state-off', 'state-blink');
    pillEl.classList.add('state-' + resolved);
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
      setStatusRunning('status-led', messages.reconfiguring);
    }
    return fetch('/api/builtin-led-config?gpio=' + encodeURIComponent(value))
      .then(r => r.json())
      .then(d => {
        if (d && d.success) {
          lastBuiltinGPIO = value;
          const detail = d.message && d.message.length ? d.message : messages.configUpdated;
          setStatusSuccess('status-led', detail);
          updateLedSummary(detail);
          return true;
        }
        const errMessage = d && d.message ? d.message : messages.configError;
        if (manual) {
          setStatusError('status-led', errMessage);
          updateLedSummary(errMessage);
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
    setStatusRunning('status-led', messages.reconfiguring);
    return applyBuiltinConfig(false).catch(err => {
      setStatusError('status-led', err && err.message ? err.message : messages.configError);
      updateLedSummary(err && err.message ? err.message : messages.configError);
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
      setStatusRunning('status-neopixel', messages.reconfiguring);
    }
    const url = `/api/neopixel-config?gpio=${encodeURIComponent(gpioValue)}&count=${encodeURIComponent(countValue)}`;
    return fetch(url)
      .then(r => r.json())
      .then(d => {
        if (d && d.success) {
          lastNeoGPIO = gpioValue;
          lastNeoCount = countValue;
          const detail = d.message && d.message.length ? d.message : messages.configUpdated;
          setStatusSuccess('status-neopixel', detail);
          updateNeoSummary(detail);
          return true;
        }
        const msg = d && d.message ? d.message : messages.configError;
        if (manual) {
          setStatusError('status-neopixel', msg);
          updateNeoSummary(msg);
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
    setStatusRunning('status-neopixel', messages.reconfiguring);
    return applyNeoPixelConfig(false).catch(err => {
      setStatusError('status-neopixel', err && err.message ? err.message : messages.configError);
      updateNeoSummary(err && err.message ? err.message : messages.configError);
      throw err;
    });
  }

  function applyOLEDConfig(manual = true) {
    const status = document.getElementById('status-oled');
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
    if (manual) {
      setStatusRunning('status-oled', messages.reconfiguring);
    }
    const url = `/api/oled-config?sda=${encodeURIComponent(sda)}&scl=${encodeURIComponent(scl)}`;
    return fetch(url)
      .then(r => r.json())
      .then(d => {
        if (d && d.success) {
          lastOledConfig = { sda, scl };
          const detail = d.message && d.message.length ? d.message : messages.configUpdated;
          setStatusSuccess('status-oled', detail);
          updateOledSummary(detail);
          const pins = document.getElementById('oled-pins');
          if (pins) {
            pins.textContent = `SDA:${sda} SCL:${scl}`;
          }
          return true;
        }
        const msg = d && d.message ? d.message : messages.configError;
        setStatusError('status-oled', msg);
        updateOledSummary(msg);
        throw new Error(msg);
      });
  }

  function ensureOLEDConfigured() {
    const sdaEl = document.getElementById('oledSDA');
    const sclEl = document.getElementById('oledSCL');
    const status = document.getElementById('status-oled');
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
    setStatusRunning('status-oled', messages.reconfiguring);
    return applyOLEDConfig(false).catch(err => {
      setStatusError('status-oled', err && err.message ? err.message : messages.configError);
      updateOledSummary(err && err.message ? err.message : messages.configError);
      throw err;
    });
  }

  function configBuiltinLED() {
    applyBuiltinConfig(true).catch(err => {
      console.error('LED config', err);
      const message = err && err.message ? err.message : messages.configError;
      setStatusError('status-led', message);
      updateLedSummary(message);
    });
  }

  function testBuiltinLED() {
    ensureBuiltinConfigured()
      .then(() => {
        setStatusRunning('status-led', messages.testingLed);
        return fetch('/api/builtin-led-test');
      })
      .then(r => r.json())
      .then(d => {
        if (!d) return;
        if (d.success === false) {
          setStatusError('status-led', d.result || messages.configError);
          updateLedSummary(d.result || messages.configError);
        } else {
          setStatusSuccess('status-led', d.result || messages.ok);
          updateLedSummary(d.result || messages.ok);
        }
      })
      .catch(err => {
        console.error('LED test', err);
        setStatusError('status-led', err && err.message ? err.message : messages.configError);
        updateLedSummary(err && err.message ? err.message : messages.configError);
      });
  }

  function ledBlink() {
    ensureBuiltinConfigured()
      .then(() => {
        setStatusRunning('status-led', messages.testingLedEffect);
        return fetch('/api/builtin-led-control?action=blink');
      })
      .then(r => r.json())
      .then(d => {
        if (!d || d.success === false) {
          setStatusError('status-led', d && d.message ? d.message : messages.configError);
          updateLedSummary(d && d.message ? d.message : messages.configError);
        } else {
          setStatusSuccess('status-led', d.message || messages.ok);
          updateLedSummary(d.message || messages.ok);
        }
      })
      .catch(err => {
        console.error('LED blink', err);
        setStatusError('status-led', err && err.message ? err.message : messages.configError);
        updateLedSummary(err && err.message ? err.message : messages.configError);
      });
  }

  function ledFade() {
    ensureBuiltinConfigured()
      .then(() => {
        setStatusRunning('status-led', messages.testingLedFade);
        return fetch('/api/builtin-led-control?action=fade');
      })
      .then(r => r.json())
      .then(d => {
        if (!d || d.success === false) {
          setStatusError('status-led', d && d.message ? d.message : messages.configError);
          updateLedSummary(d && d.message ? d.message : messages.configError);
        } else {
          setStatusSuccess('status-led', d.message || messages.ok);
          updateLedSummary(d.message || messages.ok);
        }
      })
      .catch(err => {
        console.error('LED fade', err);
        setStatusError('status-led', err && err.message ? err.message : messages.configError);
        updateLedSummary(err && err.message ? err.message : messages.configError);
      });
  }

  function ledOff() {
    ensureBuiltinConfigured()
      .then(() => {
        setStatusRunning('status-led', messages.testingLedOff);
        return fetch('/api/builtin-led-control?action=off');
      })
      .then(r => r.json())
      .then(d => {
        if (!d || d.success === false) {
          setStatusError('status-led', d && d.message ? d.message : messages.configError);
          updateLedSummary(d && d.message ? d.message : messages.configError);
        } else {
          setStatusSuccess('status-led', d.message || messages.ok);
          updateLedSummary(d.message || messages.ok);
        }
      })
      .catch(err => {
        console.error('LED off', err);
        setStatusError('status-led', err && err.message ? err.message : messages.configError);
        updateLedSummary(err && err.message ? err.message : messages.configError);
      });
  }

  function configNeoPixel() {
    applyNeoPixelConfig(true).catch(err => {
      console.error('NeoPixel config', err);
      const message = err && err.message ? err.message : messages.configError;
      setStatusError('status-neopixel', message);
      updateNeoSummary(message);
    });
  }

  function testNeoPixel() {
    ensureNeoPixelConfigured()
      .then(() => {
        setStatusRunning('status-neopixel', messages.testingNeopixel);
        return fetch('/api/neopixel-test');
      })
      .then(r => r.json())
      .then(d => {
        if (!d) return;
        if (d.success === false) {
          setStatusError('status-neopixel', d.result || messages.configError);
          updateNeoSummary(d.result || messages.configError);
        } else {
          setStatusSuccess('status-neopixel', d.result || messages.ok);
          updateNeoSummary(d.result || messages.ok);
        }
      })
      .catch(err => {
        console.error('NeoPixel test', err);
        setStatusError('status-neopixel', err && err.message ? err.message : messages.configError);
        updateNeoSummary(err && err.message ? err.message : messages.configError);
      });
  }

  function neoPattern(pattern) {
    ensureNeoPixelConfigured()
      .then(() => {
        const statusLabel = pattern === 'off'
          ? messages.testingNeopixelOff
          : messages.testingNeopixelEffect;
        setStatusRunning('status-neopixel', statusLabel);
        return fetch('/api/neopixel-pattern?pattern=' + encodeURIComponent(pattern));
      })
      .then(r => r.json())
      .then(d => {
        if (!d || d.success === false) {
          setStatusError('status-neopixel', d && d.message ? d.message : messages.configError);
          updateNeoSummary(d && d.message ? d.message : messages.configError);
        } else {
          setStatusSuccess('status-neopixel', d.message || messages.ok);
          updateNeoSummary(d.message || messages.ok);
        }
      })
      .catch(err => {
        console.error('NeoPixel pattern', err);
        setStatusError('status-neopixel', err && err.message ? err.message : messages.configError);
        updateNeoSummary(err && err.message ? err.message : messages.configError);
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
      .then(() => {
        setStatusRunning('status-neopixel', messages.testingNeopixelColor);
        return fetch(`/api/neopixel-color?r=${r}&g=${g}&b=${b}`);
      })
      .then(r => r.json())
      .then(d => {
        if (!d || d.success === false) {
          setStatusError('status-neopixel', d && d.message ? d.message : messages.configError);
          updateNeoSummary(d && d.message ? d.message : messages.configError);
        } else {
          setStatusSuccess('status-neopixel', d.message || messages.ok);
          updateNeoSummary(d.message || messages.ok);
        }
      })
      .catch(err => {
        console.error('NeoPixel color', err);
        setStatusError('status-neopixel', err && err.message ? err.message : messages.configError);
        updateNeoSummary(err && err.message ? err.message : messages.configError);
      });
  }

  function testOLED() {
    ensureOLEDConfigured()
      .then(() => {
        setStatusRunning('status-oled', messages.testingOled);
        return fetch('/api/oled-test');
      })
      .then(r => r.json())
      .then(d => {
        if (!d) return;
        if (d.success === false) {
          setStatusError('status-oled', d.result || messages.configError);
          updateOledSummary(d.result || messages.configError);
        } else {
          updateOledSummary(d.result || messages.ok);
          const completedMessage = currentLang === 'fr' ? 'Test OLED terminé' : 'OLED test finished';
          showStatus('status-oled', completedMessage);
        }
      })
      .catch(err => {
        console.error('OLED test', err);
        setStatusError('status-oled', err && err.message ? err.message : messages.configError);
        updateOledSummary(err && err.message ? err.message : messages.configError);
      });
  }

  function oledStep(step) {
    ensureOLEDConfigured()
      .then(() => {
        setStatusRunning('status-oled', messages.testingOledStep);
        return fetch('/api/oled-step?step=' + encodeURIComponent(step));
      })
      .then(r => r.json())
      .then(d => {
        if (!d) return;
        if (d.success) {
          setStatusSuccess('status-oled', d.message || messages.ok);
          updateOledSummary(d.message || messages.ok);
        } else {
          setStatusError('status-oled', d.message || messages.configError);
          updateOledSummary(d.message || messages.configError);
        }
      })
      .catch(err => {
        const message = err && err.message ? err.message : String(err);
        setStatusError('status-oled', message);
        updateOledSummary(message);
      });
  }

  function oledMessage() {
    const input = document.getElementById('oledMsg');
    if (!input) return;
    ensureOLEDConfigured()
      .then(() => {
        setStatusRunning('status-oled', messages.testingOledMessage);
        return fetch('/api/oled-message?message=' + encodeURIComponent(input.value || ''));
      })
      .then(r => r.json())
      .then(d => {
        if (!d) return;
        if (d.success === false) {
          setStatusError('status-oled', d.message || messages.configError);
          updateOledSummary(d.message || messages.configError);
        } else {
          setStatusSuccess('status-oled', d.message || messages.ok);
          updateOledSummary(d.message || messages.ok);
        }
      })
      .catch(err => {
        console.error('OLED message', err);
        setStatusError('status-oled', err && err.message ? err.message : messages.configError);
        updateOledSummary(err && err.message ? err.message : messages.configError);
      });
  }

  function configOLED() {
    applyOLEDConfig(true).catch(err => {
      console.error('OLED config', err);
      const message = err && err.message ? err.message : messages.configError;
      setStatusError('status-oled', message);
      updateOledSummary(message);
    });
  }

  function testADC() {
    setStatusRunning('status-adc', messages.testingAdc);
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
        setStatusSuccess('status-adc', data.result || '');
      })
      .catch(err => {
        console.error('ADC test', err);
        setStatusError('status-adc', err && err.message ? err.message : messages.configError);
      });
  }

  function testTouch() {
    setStatusRunning('status-touch', messages.testingTouch);
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
        setStatusSuccess('status-touch', data.result || '');
      })
      .catch(err => {
        console.error('Touch test', err);
        setStatusError('status-touch', err && err.message ? err.message : messages.configError);
      });
  }

  function testPWM() {
    setStatusRunning('status-pwm', messages.testingPwm);
    fetch('/api/pwm-test')
      .then(r => r.json())
      .then(d => {
        if (!d) return;
        setStatusSuccess('status-pwm', d.result || '');
      })
      .catch(err => {
        console.error('PWM test', err);
        setStatusError('status-pwm', err && err.message ? err.message : messages.configError);
      });
  }

  function scanSPI() {
    setStatusRunning('status-spi', messages.testingSpi);
    fetch('/api/spi-scan')
      .then(r => r.json())
      .then(d => {
        if (!d) return;
        setStatusSuccess('status-spi', d.info || messages.ok);
      })
      .catch(err => {
        console.error('SPI scan', err);
        setStatusError('status-spi', err && err.message ? err.message : messages.configError);
      });
  }

  function listPartitions() {
    showStatus('status-partitions', messages.testingPartitions);
    fetch('/api/partitions-list')
      .then(r => r.json())
      .then(d => {
        setHTML('partitions-results', d.partitions || '');
        const doneMessage = currentLang === 'fr' ? 'Analyse des partitions terminée' : 'Partition analysis finished';
        setStatusSuccess('status-partitions', doneMessage);
      })
      .catch(err => {
        console.error('Partitions list', err);
        setStatusError('status-partitions', err && err.message ? err.message : messages.configError);
      });
  }

  function stressTest() {
    setStatusRunning('status-stress', messages.testingStress);
    fetch('/api/stress-test')
      .then(r => r.json())
      .then(d => {
        if (!d) return;
        setStatusSuccess('status-stress', d.result || '');
      })
      .catch(err => {
        console.error('Stress test', err);
        setStatusError('status-stress', err && err.message ? err.message : messages.configError);
      });
  }

  function testAllGPIO() {
    setStatusRunning('status-gpio', messages.testingGpio);
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
        setStatusSuccess('status-gpio', `${total} ${messages.gpio} ${messages.tested}`);
      })
      .catch(err => {
        console.error('GPIO test', err);
        setStatusError('status-gpio', err && err.message ? err.message : messages.configError);
      });
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
    const wifiPill = document.getElementById('wifi-status-pill');
    const wifiFlag = Object.prototype.hasOwnProperty.call(wifi, 'available') ? toBool(wifi.available) : true;
    const wifiStation = toBool(wifi.station_connected);
    const wifiAP = toBool(wifi.ap_active);
    const wifiDriverInitialized = toBool(wifi.driver_initialized);
    const wifiConnected = Object.prototype.hasOwnProperty.call(wifi, 'connected')
      ? toBool(wifi.connected)
      : (wifiStation || wifiAP);
    const wifiAvailable = wifiFlag || wifiStation || wifiAP || wifiDriverInitialized;

    const wifiHeaderStateRaw = typeof wifi.header_state === 'string' ? wifi.header_state.toLowerCase() : '';
    let wifiStateClass = ['on', 'mid', 'off', 'blink'].includes(wifiHeaderStateRaw) ? wifiHeaderStateRaw : null;
    if (!wifiStateClass) {
      if (!wifiAvailable) {
        wifiStateClass = 'off';
      } else if (wifiStation || wifiConnected) {
        wifiStateClass = 'on';
      } else if (wifiAP) {
        wifiStateClass = 'mid';
      } else {
        wifiStateClass = 'blink';
      }
    }
    const wifiHeaderText = (typeof wifi.header_text === 'string' && wifi.header_text.length)
      ? wifi.header_text
      : (wifiStateClass === 'on'
        ? labels.connected
        : wifiStateClass === 'mid'
          ? indicator.ap
          : wifiStateClass === 'blink'
            ? messages.wifiWaiting
            : indicator.unavailable);

    if (wifiLabel) {
      wifiLabel.textContent = wifiHeaderText;
    }
    updateDotState(wifiDot, wifiStateClass);
    updatePillState(wifiPill, wifiStateClass);

    let connectionText;
    switch (wifiStateClass) {
      case 'on':
        connectionText = `✅ ${wifiHeaderText}`;
        break;
      case 'mid':
        connectionText = `🟡 ${wifiHeaderText}`;
        break;
      case 'blink':
        connectionText = `⏳ ${wifiHeaderText}`;
        break;
      default:
        connectionText = `❌ ${wifiHeaderText}`;
        break;
    }
    updateText('wifi-connection-state', connectionText);

    let ssidValue = '';
    if (wifiStation && wifi.ssid && wifi.ssid.length) {
      ssidValue = wifi.ssid;
    } else if (wifiAP && wifi.ap_ssid && wifi.ap_ssid.length) {
      ssidValue = wifi.ap_ssid;
    }
    updateText('wifi-ssid', ssidValue || fallbackChar);

    const channel = typeof wifi.channel === 'number' && wifi.channel > 0 ? labels.channelPrefix + wifi.channel : '';
    updateText('wifi-channel', wifiStation ? channel : fallbackChar);
    updateText('wifi-mode', wifiAvailable && wifi.mode ? wifi.mode : fallbackChar);
    updateText('wifi-sleep', wifiAvailable && wifi.sleep ? wifi.sleep : fallbackChar);
    updateText('wifi-band', wifiAvailable && wifi.band ? wifi.band : fallbackChar);
    updateText('wifi-band-mode', wifiAvailable && wifi.band_mode ? wifi.band_mode : fallbackChar);
    const txCode = typeof wifi.tx_power_code === 'number' ? wifi.tx_power_code : null;
    const txPower = typeof wifi.tx_power_dbm === 'number'
      ? `${Number(wifi.tx_power_dbm).toFixed(1)} dBm (${txCode !== null ? txCode : 'n/a'})`
      : '';
    updateText('wifi-tx-power', wifiAvailable && txPower.length ? txPower : fallbackChar);
    updateText('wifi-hostname', wifi.hostname && wifi.hostname.length ? wifi.hostname : fallbackChar);

    let ipDisplay = fallbackChar;
    if (!wifiAvailable) {
      ipDisplay = fallbackChar;
      updateText('wifi-rssi', fallbackChar);
      updateText('wifi-quality', fallbackChar);
      updateText('wifi-ip', fallbackChar);
      updateText('wifi-subnet', fallbackChar);
      updateText('wifi-gateway', fallbackChar);
      updateText('wifi-dns', fallbackChar);
    } else if (wifiStation) {
      updateText('wifi-rssi', `${wifi.rssi} dBm`);
      updateText('wifi-quality', wifi.quality || fallbackChar);
      ipDisplay = wifi.ip && wifi.ip.length ? wifi.ip : fallbackChar;
      updateText('wifi-ip', ipDisplay);
      updateText('wifi-subnet', wifi.subnet || fallbackChar);
      updateText('wifi-gateway', wifi.gateway || fallbackChar);
      updateText('wifi-dns', wifi.dns || fallbackChar);
    } else if (wifiAP) {
      updateText('wifi-rssi', fallbackChar);
      updateText('wifi-quality', fallbackChar);
      ipDisplay = wifi.ap_ip && wifi.ap_ip.length ? wifi.ap_ip : fallbackChar;
      updateText('wifi-ip', ipDisplay);
      updateText('wifi-subnet', fallbackChar);
      updateText('wifi-gateway', fallbackChar);
      updateText('wifi-dns', fallbackChar);
    } else {
      ipDisplay = fallbackChar;
      updateText('wifi-rssi', fallbackChar);
      updateText('wifi-quality', fallbackChar);
      updateText('wifi-ip', fallbackChar);
      updateText('wifi-subnet', fallbackChar);
      updateText('wifi-gateway', fallbackChar);
      updateText('wifi-dns', fallbackChar);
    }
    updateText('ipAddress', ipDisplay);

    const btClassic = toBool(bt.classic);
    const btBle = toBool(bt.ble);
    const compileEnabled = toBool(bt.compile_enabled);
    const controllerEnabled = toBool(bt.controller_enabled);
    const controllerInitialized = toBool(bt.controller_initialized);
    const lastTestSuccess = toBool(bt.last_test_success);

    const capabilities = [];
    if (btClassic) {
      capabilities.push(currentLang === 'fr' ? 'Classique' : 'Classic');
    }
    if (btBle) {
      capabilities.push('BLE');
    }
    updateText('bluetooth-capabilities', capabilities.length ? capabilities.join(', ') : fallbackChar);
    updateText('bluetooth-controller', bt.controller || fallbackChar);
    const lastMessage = typeof bt.last_test_message === 'string' ? bt.last_test_message : '';
    updateText('bluetooth-last-test', lastMessage.length ? lastMessage : fallbackChar);
    const hasMeaningfulTest = lastMessage.length && lastMessage !== messages.notTested;
    if (hasMeaningfulTest) {
      if (!lastTestSuccess) {
        setStatusError('status-bt', lastMessage);
      } else {
        setStatusSuccess('status-bt', lastMessage);
      }
    } else if (!compileEnabled && bt.hint && bt.hint.length) {
      setStatusError('status-bt', bt.hint);
    } else {
      showStatus('status-bt', messages.notTested);
    }
    const hint = bt.hint && bt.hint.length ? bt.hint : '\u00A0';
    setText('bluetooth-hint', hint);

    const btDot = document.getElementById('bt-status-dot');
    const btLabel = document.getElementById('bt-status-label');
    const btPill = document.getElementById('bt-status-pill');
    const hasBluetoothHardware = btClassic || btBle;
    const btHeaderStateRaw = typeof bt.header_state === 'string' ? bt.header_state.toLowerCase() : '';
    let btStateClass = ['on', 'mid', 'off', 'blink'].includes(btHeaderStateRaw) ? btHeaderStateRaw : null;
    if (!btStateClass) {
      if (!compileEnabled || !hasBluetoothHardware) {
        btStateClass = 'off';
      } else if (controllerEnabled && !hasMeaningfulTest) {
        btStateClass = 'mid';
      } else if (controllerInitialized) {
        btStateClass = 'blink';
      } else {
        btStateClass = 'off';
      }
    }
    const btHeaderText = (typeof bt.header_text === 'string' && bt.header_text.length)
      ? bt.header_text
      : (btStateClass === 'on'
        ? (bt.controller && bt.controller.length ? bt.controller : labels.connected)
        : btStateClass === 'mid'
          ? (bt.controller && bt.controller.length ? bt.controller : indicator.bluetoothLabel)
          : btStateClass === 'blink'
            ? messages.testingBluetooth
            : indicator.unavailable);

    if (btLabel) {
      btLabel.textContent = btHeaderText;
    }
    updateDotState(btDot, btStateClass);
    updatePillState(btPill, btStateClass);
  }

  function testBluetooth() {
    setStatusRunning('status-bt', messages.testingBluetooth);
    fetch('/api/bluetooth-test')
      .then(r => r.json())
      .then(d => {
        if (!d) return;
        if (d.success === false) {
          setStatusError('status-bt', d.message || messages.unknown);
        } else {
          setStatusSuccess('status-bt', d.message || messages.ok);
        }
        refreshWirelessInfo();
      })
      .catch(err => {
        const message = err && err.message ? err.message : String(err);
        setStatusError('status-bt', message);
      });
  }

  function scanWiFi() {
    setStatusRunning('status-wifi', messages.scanning);
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
        setStatusSuccess('status-wifi', `${total} ${messages.networks}`);
      })
      .catch(err => {
        console.error('WiFi scan', err);
        setStatusError('status-wifi', err && err.message ? err.message : messages.configError);
      });
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

  function exportExcel() {
    const running = currentLang === 'fr' ? 'Export Excel en cours...' : 'Excel export in progress...';
    const done = currentLang === 'fr' ? 'Export Excel terminé' : 'Excel export finished';
    showStatus('status-export', running);
    fetch('/export/csv')
      .then(resp => {
        if (!resp.ok) {
          throw new Error(`${resp.status} ${resp.statusText}`.trim());
        }
        return resp.blob();
      })
      .then(blob => {
        const url = URL.createObjectURL(blob);
        const anchor = document.createElement('a');
        anchor.href = url;
        const stamp = new Date().toISOString().replace(/[:.]/g, '-');
        anchor.download = `esp32-diagnostic-${stamp}.csv`;
        document.body.appendChild(anchor);
        anchor.click();
        document.body.removeChild(anchor);
        URL.revokeObjectURL(url);
        showStatus('status-export', done);
      })
      .catch(err => {
        console.error('Export Excel', err);
        setStatusError('status-export', err && err.message ? err.message : messages.configError);
      });
  }

  function runBenchmarks() {
    const runningMessage = messages.testingBenchmarks;
    showStatus('status-perf-cpu', runningMessage);
    showStatus('status-perf-mem', runningMessage);
    showStatus('status-perf-panel-cpu', runningMessage);
    showStatus('status-perf-panel-mem', runningMessage);
    setText('cpu-bench', runningMessage);
    setText('mem-bench', runningMessage);
    setText('cpu-perf', runningMessage);
    setText('mem-speed', runningMessage);
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
        const completion = currentLang === 'fr'
          ? {
              cpu: 'CPU Benchmark terminé',
              mem: 'Mémoire Benchmark terminée',
              panelCpu: 'Performance CPU mise à jour',
              panelMem: 'Vitesse mémoire mise à jour'
            }
          : {
              cpu: 'CPU benchmark finished',
              mem: 'Memory benchmark finished',
              panelCpu: 'CPU performance updated',
              panelMem: 'Memory speed updated'
            };
        showStatus('status-perf-cpu', completion.cpu);
        showStatus('status-perf-mem', completion.mem);
        showStatus('status-perf-panel-cpu', completion.panelCpu);
        showStatus('status-perf-panel-mem', completion.panelMem);
      })
      .catch(err => {
        console.error('Benchmarks', err);
        const message = err && err.message ? err.message : messages.configError;
        setStatusError('status-perf-cpu', message);
        setStatusError('status-perf-mem', message);
        setStatusError('status-perf-panel-cpu', message);
        setStatusError('status-perf-panel-mem', message);
      });
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
    setInterval(refreshWirelessInfo, {{WIRELESS_REFRESH}});
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
    runBenchmarks,
    exportExcel
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
  script.replace(F("{{AP_STATE}}"), escapeForJS(String(T().indicator_ap)));
  script.replace(F("{{OFFLINE}}"), escapeForJS(String(T().disconnected)));
  script.replace(F("{{UNAVAILABLE}}"), escapeForJS(String(T().indicator_unavailable)));
  String channelPrefix = (currentLanguage == LANG_FR) ? String("Canal ") : String("Ch ");
  script.replace(F("{{CHANNEL_PREFIX}}"), escapeForJS(channelPrefix));
  script.replace(F("{{TESTING}}"), escapeForJS(String(T().testing)));
  script.replace(F("{{TESTING_OLED}}"), escapeForJS(String(T().testing_oled)));
  script.replace(F("{{RECONFIGURING}}"), escapeForJS(String(T().reconfiguring)));
  script.replace(F("{{CONFIG_UPDATED}}"), escapeForJS(String(T().config_updated)));
  script.replace(F("{{CONFIG_ERROR}}"), escapeForJS(String(T().config_error)));
  script.replace(F("{{ERROR_PREFIX}}"), escapeForJS(String(T().error_prefix)));
  script.replace(F("{{TESTING_LED}}"), escapeForJS(String(T().testing_led)));
  script.replace(F("{{TESTING_LED_EFFECT}}"), escapeForJS(String(T().testing_led_effect)));
  script.replace(F("{{TESTING_LED_FADE}}"), escapeForJS(String(T().testing_led_fade)));
  script.replace(F("{{TESTING_LED_OFF}}"), escapeForJS(String(T().testing_led_off)));
  script.replace(F("{{TESTING_NEOPIXEL}}"), escapeForJS(String(T().testing_neopixel)));
  script.replace(F("{{TESTING_NEOPIXEL_EFFECT}}"), escapeForJS(String(T().testing_neopixel_effect)));
  script.replace(F("{{TESTING_NEOPIXEL_COLOR}}"), escapeForJS(String(T().testing_neopixel_color)));
  script.replace(F("{{TESTING_NEOPIXEL_OFF}}"), escapeForJS(String(T().testing_neopixel_off)));
  script.replace(F("{{TESTING_OLED_STEP}}"), escapeForJS(String(T().testing_oled_step)));
  script.replace(F("{{TESTING_OLED_MESSAGE}}"), escapeForJS(String(T().testing_oled_message)));
  script.replace(F("{{TESTING_ADC}}"), escapeForJS(String(T().testing_adc)));
  script.replace(F("{{TESTING_TOUCH}}"), escapeForJS(String(T().testing_touch)));
  script.replace(F("{{TESTING_PWM}}"), escapeForJS(String(T().testing_pwm)));
  script.replace(F("{{TESTING_SPI}}"), escapeForJS(String(T().testing_spi)));
  script.replace(F("{{TESTING_PARTITIONS}}"), escapeForJS(String(T().testing_partitions)));
  script.replace(F("{{TESTING_STRESS}}"), escapeForJS(String(T().testing_stress)));
  script.replace(F("{{TESTING_GPIO}}"), escapeForJS(String(T().testing_gpio)));
  script.replace(F("{{TESTING_BLUETOOTH}}"), escapeForJS(String(T().testing_bluetooth)));
  script.replace(F("{{TESTING_BENCHMARKS}}"), escapeForJS(String(T().testing_benchmarks)));
  script.replace(F("{{WIFI_WAITING}}"), escapeForJS(String(T().wifi_waiting)));
  script.replace(F("{{SCANNING}}"), escapeForJS(String(T().scanning)));
  script.replace(F("{{OK}}"), escapeForJS(String(T().ok)));
  script.replace(F("{{FAIL}}"), escapeForJS(String(T().fail)));
  script.replace(F("{{COMPLETED}}"), escapeForJS(String(T().completed)));
  script.replace(F("{{GPIO}}"), escapeForJS(String(T().gpio)));
  script.replace(F("{{TESTED}}"), escapeForJS(String(T().tested)));
  script.replace(F("{{UNKNOWN}}"), escapeForJS(String(T().unknown)));
  script.replace(F("{{NETWORKS}}"), escapeForJS(String(T().networks)));
  script.replace(F("{{DEVICES}}"), escapeForJS(String(T().devices)));
  script.replace(F("{{NOT_TESTED}}"), escapeForJS(String(T().not_tested)));
  script.replace(F("{{WIRELESS_REFRESH}}"), String(WIRELESS_STATUS_REFRESH_MS));

  return script;
}

#endif  // APP_SCRIPT_H
