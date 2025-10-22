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
    testingNeopixelGeneric: "{{TESTING_NEOPIXEL_GENERIC}}",
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
    notTested: "{{NOT_TESTED}}",
    ledTestDone: "{{LED_TEST_DONE}}",
    ledBlinkDone: "{{LED_BLINK_DONE}}",
    ledFadeDone: "{{LED_FADE_DONE}}",
    ledOffDone: "{{LED_OFF_DONE}}",
    neoTestDone: "{{NEO_TEST_DONE}}",
    neoRainbowDone: "{{NEO_RAINBOW_DONE}}",
    neoColorDone: "{{NEO_COLOR_DONE}}",
    neoOffDone: "{{NEO_OFF_DONE}}",
    oledTestDone: "{{OLED_TEST_DONE}}",
    adcTestDone: "{{ADC_TEST_DONE}}",
    touchTestDone: "{{TOUCH_TEST_DONE}}",
    pwmTestDone: "{{PWM_TEST_DONE}}",
    spiTestDone: "{{SPI_TEST_DONE}}",
    partitionsDone: "{{PARTITIONS_DONE}}",
    stressTestDone: "{{STRESS_TEST_DONE}}",
    gpioTestDone: "{{GPIO_TEST_DONE}}",
    wifiScanDone: "{{WIFI_SCAN_DONE}}",
    btTestDone: "{{BT_TEST_DONE}}",
    benchmarksDone: "{{BENCHMARKS_DONE}}",
    exportProgress: "{{EXPORT_PROGRESS}}",
    exportDone: "{{EXPORT_DONE}}"
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

  const statusClearTimers = {};
  const statusProgressTimers = {};

  function clearPendingStatus(targetId) {
    if (statusProgressTimers[targetId]) {
      clearTimeout(statusProgressTimers[targetId]);
      delete statusProgressTimers[targetId];
    }
    if (statusClearTimers[targetId]) {
      clearTimeout(statusClearTimers[targetId]);
      delete statusClearTimers[targetId];
    }
  }

  function showStatus(targetId, message, duration = 0) {
    const el = document.getElementById(targetId);
    if (!el) return;
    clearPendingStatus(targetId);
    el.textContent = typeof message === 'string' ? message : '';
    if (duration > 0) {
      statusClearTimers[targetId] = setTimeout(() => {
        const ref = document.getElementById(targetId);
        if (ref) {
          ref.textContent = '';
        }
        delete statusClearTimers[targetId];
      }, duration);
    }
  }

  function clearStatus(targetId) {
    showStatus(targetId, '', 0);
  }

  function showProgressAndResult(targetId, inProgressText, resultText, testDuration = 3000) {
    if (statusProgressTimers[targetId]) {
      clearTimeout(statusProgressTimers[targetId]);
      delete statusProgressTimers[targetId];
    }
    showStatus(targetId, inProgressText);
    if (testDuration > 0) {
      statusProgressTimers[targetId] = setTimeout(() => {
        delete statusProgressTimers[targetId];
        showStatus(targetId, resultText, 4000);
      }, testDuration);
    }
  }

  const setStatusRunning = (id, label) => {
    showStatus(id, label || messages.testing);
  };

  const setStatusSuccess = (id, detail) => {
    const message = detail && detail.length ? detail : messages.completed;
    showStatus(id, message, 4000);
  };

  const setStatusError = (id, detail) => {
    const prefix = messages.errorPrefix || '';
    const raw = detail && detail.length ? detail : messages.configError;
    const text = raw.startsWith(prefix) ? raw : `${prefix}${raw}`;
    showStatus(id, text, 6000);
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

  const STATE_CLASSES = ['state-off', 'state-mid', 'state-on'];

  const normalizeStateClass = raw => {
    if (typeof raw !== 'string') {
      return 'state-off';
    }
    const trimmed = raw.trim().toLowerCase();
    if (!trimmed.length) {
      return 'state-off';
    }
    const prefixed = trimmed.startsWith('state-') ? trimmed : `state-${trimmed}`;
    return STATE_CLASSES.includes(prefixed) ? prefixed : 'state-off';
  };

  function applyIndicatorClass(moduleId, stateClass) {
    const dotEl = document.getElementById(`${moduleId}-status-dot`);
    if (!dotEl) return;
    const normalized = normalizeStateClass(stateClass);
    dotEl.classList.remove(...STATE_CLASSES);
    dotEl.classList.add(normalized);
  }

  function applyCardClass(moduleId, stateClass) {
    const pillEl = document.getElementById(`${moduleId}-status-pill`);
    if (!pillEl) return;
    const normalized = normalizeStateClass(stateClass);
    pillEl.classList.remove(...STATE_CLASSES);
    pillEl.classList.add(normalized);
  }

  function setStatusText(moduleId, text) {
    const labelEl = document.getElementById(`${moduleId}-status-label`);
    if (!labelEl) return;
    labelEl.textContent = typeof text === 'string' ? text : '';
  }

  function setModuleState(moduleId, stateClass, text) {
    const normalized = normalizeStateClass(stateClass);
    applyIndicatorClass(moduleId, normalized);
    applyCardClass(moduleId, normalized);
    setStatusText(moduleId, text);
    return normalized;
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
        showProgressAndResult('status-led', messages.testingLed, messages.ledTestDone, 0);
        return fetch('/api/builtin-led-test');
      })
      .then(r => r.json())
      .then(d => {
        if (!d) return;
        if (d.success === false) {
          const failure = d.result || messages.configError;
          showStatus('status-led', failure, 6000);
          updateLedSummary(failure);
        } else {
          updateLedSummary(d.result || messages.ok);
          showStatus('status-led', messages.ledTestDone, 4000);
        }
      })
      .catch(err => {
        console.error('LED test', err);
        const message = err && err.message ? err.message : messages.configError;
        showStatus('status-led', message, 6000);
        updateLedSummary(message);
      });
  }

  function ledBlink() {
    ensureBuiltinConfigured()
      .then(() => {
        showProgressAndResult('status-led', messages.testingLedEffect, messages.ledBlinkDone, 0);
        return fetch('/api/builtin-led-control?action=blink');
      })
      .then(r => r.json())
      .then(d => {
        if (!d || d.success === false) {
          const failure = d && d.message ? d.message : messages.configError;
          showStatus('status-led', failure, 6000);
          updateLedSummary(failure);
        } else {
          updateLedSummary(d.message || messages.ok);
          showStatus('status-led', messages.ledBlinkDone, 4000);
        }
      })
      .catch(err => {
        console.error('LED blink', err);
        const message = err && err.message ? err.message : messages.configError;
        showStatus('status-led', message, 6000);
        updateLedSummary(message);
      });
  }

  function ledFade() {
    ensureBuiltinConfigured()
      .then(() => {
        showProgressAndResult('status-led', messages.testingLedFade, messages.ledFadeDone, 0);
        return fetch('/api/builtin-led-control?action=fade');
      })
      .then(r => r.json())
      .then(d => {
        if (!d || d.success === false) {
          const failure = d && d.message ? d.message : messages.configError;
          showStatus('status-led', failure, 6000);
          updateLedSummary(failure);
        } else {
          updateLedSummary(d.message || messages.ok);
          showStatus('status-led', messages.ledFadeDone, 4000);
        }
      })
      .catch(err => {
        console.error('LED fade', err);
        const message = err && err.message ? err.message : messages.configError;
        showStatus('status-led', message, 6000);
        updateLedSummary(message);
      });
  }

  function ledOff() {
    ensureBuiltinConfigured()
      .then(() => {
        showProgressAndResult('status-led', messages.testingLedOff, messages.ledOffDone, 0);
        return fetch('/api/builtin-led-control?action=off');
      })
      .then(r => r.json())
      .then(d => {
        if (!d || d.success === false) {
          const failure = d && d.message ? d.message : messages.configError;
          showStatus('status-led', failure, 6000);
          updateLedSummary(failure);
        } else {
          updateLedSummary(d.message || messages.ok);
          showStatus('status-led', messages.ledOffDone, 4000);
        }
      })
      .catch(err => {
        console.error('LED off', err);
        const message = err && err.message ? err.message : messages.configError;
        showStatus('status-led', message, 6000);
        updateLedSummary(message);
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
        showProgressAndResult('status-neopixel', messages.testingNeopixel, messages.neoTestDone, 0);
        return fetch('/api/neopixel-test');
      })
      .then(r => r.json())
      .then(d => {
        if (!d) return;
        if (d.success === false) {
          const failure = d.result || messages.configError;
          showStatus('status-neopixel', failure, 6000);
          updateNeoSummary(failure);
        } else {
          updateNeoSummary(d.result || messages.ok);
          showStatus('status-neopixel', messages.neoTestDone, 4000);
        }
      })
      .catch(err => {
        console.error('NeoPixel test', err);
        const message = err && err.message ? err.message : messages.configError;
        showStatus('status-neopixel', message, 6000);
        updateNeoSummary(message);
      });
  }

  function neoPattern(pattern) {
    ensureNeoPixelConfigured()
      .then(() => {
        const variants = {
          rainbow: { progress: messages.testingNeopixelEffect, done: messages.neoRainbowDone },
          off: { progress: messages.testingNeopixelOff, done: messages.neoOffDone }
        };
        const selected = Object.prototype.hasOwnProperty.call(variants, pattern)
          ? variants[pattern]
          : { progress: messages.testingNeopixelGeneric, done: messages.neoTestDone };
        showProgressAndResult('status-neopixel', selected.progress, selected.done, 0);
        return fetch('/api/neopixel-pattern?pattern=' + encodeURIComponent(pattern));
      })
      .then(r => r.json())
      .then(d => {
        if (!d || d.success === false) {
          const failure = d && d.message ? d.message : messages.configError;
          showStatus('status-neopixel', failure, 6000);
          updateNeoSummary(failure);
        } else {
          updateNeoSummary(d.message || messages.ok);
          const variants = {
            rainbow: messages.neoRainbowDone,
            off: messages.neoOffDone
          };
          const finalMessage = Object.prototype.hasOwnProperty.call(variants, pattern)
            ? variants[pattern]
            : messages.neoTestDone;
          showStatus('status-neopixel', finalMessage, 4000);
        }
      })
      .catch(err => {
        console.error('NeoPixel pattern', err);
        const message = err && err.message ? err.message : messages.configError;
        showStatus('status-neopixel', message, 6000);
        updateNeoSummary(message);
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
        showProgressAndResult('status-neopixel', messages.testingNeopixelColor, messages.neoColorDone, 0);
        return fetch(`/api/neopixel-color?r=${r}&g=${g}&b=${b}`);
      })
      .then(r => r.json())
      .then(d => {
        if (!d || d.success === false) {
          const failure = d && d.message ? d.message : messages.configError;
          showStatus('status-neopixel', failure, 6000);
          updateNeoSummary(failure);
        } else {
          updateNeoSummary(d.message || messages.ok);
          showStatus('status-neopixel', messages.neoColorDone, 4000);
        }
      })
      .catch(err => {
        console.error('NeoPixel color', err);
        const message = err && err.message ? err.message : messages.configError;
        showStatus('status-neopixel', message, 6000);
        updateNeoSummary(message);
      });
  }

  function testOLED() {
    ensureOLEDConfigured()
      .then(() => {
        showProgressAndResult('status-oled', messages.testingOled, messages.oledTestDone, 0);
        return fetch('/api/oled-test');
      })
      .then(r => r.json())
      .then(d => {
        if (!d) return;
        if (d.success === false) {
          const failure = d.result || messages.configError;
          showStatus('status-oled', failure, 6000);
          updateOledSummary(failure);
        } else {
          updateOledSummary(d.result || messages.ok);
          showStatus('status-oled', messages.oledTestDone, 4000);
        }
      })
      .catch(err => {
        console.error('OLED test', err);
        const message = err && err.message ? err.message : messages.configError;
        showStatus('status-oled', message, 6000);
        updateOledSummary(message);
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
    showProgressAndResult('status-adc', messages.testingAdc, messages.adcTestDone, 0);
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
        showStatus('status-adc', messages.adcTestDone, 4000);
      })
      .catch(err => {
        console.error('ADC test', err);
        const message = err && err.message ? err.message : messages.configError;
        showStatus('status-adc', message, 6000);
      });
  }

  function testTouch() {
    showProgressAndResult('status-touch', messages.testingTouch, messages.touchTestDone, 0);
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
        showStatus('status-touch', messages.touchTestDone, 4000);
      })
      .catch(err => {
        console.error('Touch test', err);
        const message = err && err.message ? err.message : messages.configError;
        showStatus('status-touch', message, 6000);
      });
  }

  function testPWM() {
    showProgressAndResult('status-pwm', messages.testingPwm, messages.pwmTestDone, 0);
    fetch('/api/pwm-test')
      .then(r => r.json())
      .then(d => {
        if (!d) return;
        showStatus('status-pwm', messages.pwmTestDone, 4000);
      })
      .catch(err => {
        console.error('PWM test', err);
        const message = err && err.message ? err.message : messages.configError;
        showStatus('status-pwm', message, 6000);
      });
  }

  function scanSPI() {
    showProgressAndResult('status-spi', messages.testingSpi, messages.spiTestDone, 0);
    fetch('/api/spi-scan')
      .then(r => r.json())
      .then(d => {
        if (!d) return;
        showStatus('status-spi', messages.spiTestDone, 4000);
      })
      .catch(err => {
        console.error('SPI scan', err);
        const message = err && err.message ? err.message : messages.configError;
        showStatus('status-spi', message, 6000);
      });
  }

  function listPartitions() {
    showProgressAndResult('status-partitions', messages.testingPartitions, messages.partitionsDone, 0);
    fetch('/api/partitions-list')
      .then(r => r.json())
      .then(d => {
        setHTML('partitions-results', d.partitions || '');
        showStatus('status-partitions', messages.partitionsDone, 4000);
      })
      .catch(err => {
        console.error('Partitions list', err);
        const message = err && err.message ? err.message : messages.configError;
        showStatus('status-partitions', message, 6000);
      });
  }

  function stressTest() {
    showProgressAndResult('status-stress', messages.testingStress, messages.stressTestDone, 0);
    fetch('/api/stress-test')
      .then(r => r.json())
      .then(d => {
        if (!d) return;
        showStatus('status-stress', messages.stressTestDone, 4000);
      })
      .catch(err => {
        console.error('Stress test', err);
        const message = err && err.message ? err.message : messages.configError;
        showStatus('status-stress', message, 6000);
      });
  }

  function testAllGPIO() {
    showProgressAndResult('status-gpio', messages.testingGpio, messages.gpioTestDone, 0);
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
        showStatus('status-gpio', messages.gpioTestDone, 4000);
      })
      .catch(err => {
        console.error('GPIO test', err);
        const message = err && err.message ? err.message : messages.configError;
        showStatus('status-gpio', message, 6000);
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
    const wifiFlag = Object.prototype.hasOwnProperty.call(wifi, 'available') ? toBool(wifi.available) : true;
    const wifiStation = toBool(wifi.station_connected);
    const wifiAP = toBool(wifi.ap_active);
    const wifiDriverInitialized = toBool(wifi.driver_initialized);
    const wifiConnected = Object.prototype.hasOwnProperty.call(wifi, 'connected')
      ? toBool(wifi.connected)
      : (wifiStation || wifiAP);
    const wifiAvailable = wifiFlag || wifiStation || wifiAP || wifiDriverInitialized;
    const wifiStateProvided = typeof wifi.header_state === 'string' && wifi.header_state.length;
    let wifiStateClass = wifiStateProvided ? normalizeStateClass(wifi.header_state) : '';
    if (!wifiStateProvided) {
      if (!wifiAvailable) {
        wifiStateClass = 'state-off';
      } else if (wifiStation || wifiConnected) {
        wifiStateClass = 'state-on';
      } else if (wifiAP) {
        wifiStateClass = 'state-mid';
      } else {
        wifiStateClass = 'state-mid';
      }
    }
    let wifiHeaderText = (typeof wifi.header_text === 'string' && wifi.header_text.length)
      ? wifi.header_text
      : '';
    if (!wifiHeaderText.length) {
      if (wifiStateClass === 'state-on') {
        const ssid = (wifi.ssid && wifi.ssid.length) ? wifi.ssid : '';
        wifiHeaderText = ssid.length ? `${labels.connected}: ${ssid}` : labels.connected;
      } else if (wifiStateClass === 'state-mid') {
        if (wifiAP && wifi.ap_ssid && wifi.ap_ssid.length) {
          wifiHeaderText = `${indicator.ap}: ${wifi.ap_ssid}`;
        } else {
          wifiHeaderText = messages.wifiWaiting;
        }
      } else {
        wifiHeaderText = indicator.unavailable;
      }
    }

    const appliedWifiState = setModuleState('wifi', wifiStateClass, wifiHeaderText);
    const wifiStateKey = appliedWifiState.replace('state-', '');
    let connectionText;
    switch (wifiStateKey) {
      case 'on':
        connectionText = `✅ ${wifiHeaderText}`;
        break;
      case 'mid':
        connectionText = `🟡 ${wifiHeaderText}`;
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

    const hasBluetoothHardware = btClassic || btBle;
    const btStateProvided = typeof bt.header_state === 'string' && bt.header_state.length;
    let btStateClass = btStateProvided ? normalizeStateClass(bt.header_state) : '';
    if (!btStateProvided) {
      if (!compileEnabled || !hasBluetoothHardware) {
        btStateClass = 'state-off';
      } else if (controllerEnabled && lastTestSuccess) {
        btStateClass = 'state-on';
      } else if (controllerEnabled || controllerInitialized) {
        btStateClass = 'state-mid';
      } else {
        btStateClass = 'state-off';
      }
    }
    let btHeaderText = (typeof bt.header_text === 'string' && bt.header_text.length)
      ? bt.header_text
      : '';
    if (!btHeaderText.length) {
      if (btStateClass === 'state-on') {
        btHeaderText = bt.controller && bt.controller.length ? bt.controller : labels.connected;
      } else if (btStateClass === 'state-mid') {
        btHeaderText = bt.controller && bt.controller.length ? bt.controller : indicator.bluetoothLabel;
      } else {
        btHeaderText = indicator.unavailable;
      }
    }
    setModuleState('bt', btStateClass, btHeaderText);
  }

  function testBluetooth() {
    showProgressAndResult('status-bt', messages.testingBluetooth, messages.btTestDone, 0);
    fetch('/api/bluetooth-test')
      .then(r => r.json())
      .then(d => {
        if (!d) return;
        if (d.success === false) {
          const failure = d.message || messages.unknown;
          showStatus('status-bt', failure, 6000);
        } else {
          showStatus('status-bt', messages.btTestDone, 4000);
        }
        refreshWirelessInfo();
      })
      .catch(err => {
        const message = err && err.message ? err.message : String(err);
        showStatus('status-bt', message, 6000);
      });
  }

  function scanWiFi() {
    showProgressAndResult('status-wifi', messages.scanning, messages.wifiScanDone, 0);
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
        showStatus('status-wifi', messages.wifiScanDone, 4000);
      })
      .catch(err => {
        console.error('WiFi scan', err);
        const message = err && err.message ? err.message : messages.configError;
        showStatus('status-wifi', message, 6000);
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
    showProgressAndResult('status-export', messages.exportProgress, messages.exportDone, 0);
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
        showStatus('status-export', messages.exportDone, 4000);
      })
      .catch(err => {
        console.error('Export Excel', err);
        const message = err && err.message ? err.message : messages.configError;
        showStatus('status-export', message, 6000);
      });
  }

  function runBenchmarks() {
    const runningMessage = messages.testingBenchmarks;
    ['status-perf-cpu', 'status-perf-mem', 'status-perf-panel-cpu', 'status-perf-panel-mem']
      .forEach(id => showProgressAndResult(id, runningMessage, messages.benchmarksDone, 0));
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
        ['status-perf-cpu', 'status-perf-mem', 'status-perf-panel-cpu', 'status-perf-panel-mem']
          .forEach(id => showStatus(id, messages.benchmarksDone, 4000));
      })
      .catch(err => {
        console.error('Benchmarks', err);
        const message = err && err.message ? err.message : messages.configError;
        ['status-perf-cpu', 'status-perf-mem', 'status-perf-panel-cpu', 'status-perf-panel-mem']
          .forEach(id => showStatus(id, message, 6000));
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
  script.replace(F("{{TESTING_NEOPIXEL_GENERIC}}"), escapeForJS(String(T().testing_neopixel_generic)));
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
  script.replace(F("{{LED_TEST_DONE}}"), escapeForJS(String(T().led_test_done)));
  script.replace(F("{{LED_BLINK_DONE}}"), escapeForJS(String(T().led_blink_done)));
  script.replace(F("{{LED_FADE_DONE}}"), escapeForJS(String(T().led_fade_done)));
  script.replace(F("{{LED_OFF_DONE}}"), escapeForJS(String(T().led_off_done)));
  script.replace(F("{{NEO_TEST_DONE}}"), escapeForJS(String(T().neopixel_test_done)));
  script.replace(F("{{NEO_RAINBOW_DONE}}"), escapeForJS(String(T().neopixel_rainbow_done)));
  script.replace(F("{{NEO_COLOR_DONE}}"), escapeForJS(String(T().neopixel_color_done)));
  script.replace(F("{{NEO_OFF_DONE}}"), escapeForJS(String(T().neopixel_off_done)));
  script.replace(F("{{OLED_TEST_DONE}}"), escapeForJS(String(T().oled_test_done)));
  script.replace(F("{{ADC_TEST_DONE}}"), escapeForJS(String(T().adc_test_done)));
  script.replace(F("{{TOUCH_TEST_DONE}}"), escapeForJS(String(T().touch_test_done)));
  script.replace(F("{{PWM_TEST_DONE}}"), escapeForJS(String(T().pwm_test_done)));
  script.replace(F("{{SPI_TEST_DONE}}"), escapeForJS(String(T().spi_test_done)));
  script.replace(F("{{PARTITIONS_DONE}}"), escapeForJS(String(T().partitions_done)));
  script.replace(F("{{STRESS_TEST_DONE}}"), escapeForJS(String(T().stress_test_done)));
  script.replace(F("{{GPIO_TEST_DONE}}"), escapeForJS(String(T().gpio_test_done)));
  script.replace(F("{{WIFI_SCAN_DONE}}"), escapeForJS(String(T().wifi_scan_done)));
  script.replace(F("{{BT_TEST_DONE}}"), escapeForJS(String(T().bt_test_done)));
  script.replace(F("{{BENCHMARKS_DONE}}"), escapeForJS(String(T().benchmarks_done)));
  script.replace(F("{{EXPORT_PROGRESS}}"), escapeForJS(String(T().export_progress)));
  script.replace(F("{{EXPORT_DONE}}"), escapeForJS(String(T().export_done)));
  script.replace(F("{{WIRELESS_REFRESH}}"), String(WIRELESS_STATUS_REFRESH_MS));

  return script;
}

#endif  // APP_SCRIPT_H
