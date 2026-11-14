(() => {
  const KEY = {
    up: 0xad,
    down: 0xaf,
    left: 0xac,
    right: 0xae,
    fire: 0x80 + 0x1d, // KEY_RCTRL
    use: 0x20,         // space
    strafe: 0x80 + 0x38, // KEY_RALT
    speed: 0x80 + 0x36,  // KEY_RSHIFT
    esc: 27,
    enter: 13,
    tab: 9,
    w1: "1".charCodeAt(0),
    w2: "2".charCodeAt(0),
    w3: "3".charCodeAt(0),
  };

  const statusEl = document.getElementById("status");
  const startBtn = document.getElementById("startBtn");
  const brand = document.getElementById("brand");
  const stage = document.getElementById("stage");
  const canvas = document.getElementById("screen");
  const ctx = canvas.getContext("2d", { alpha: false });
  const imageData = ctx.createImageData(320, 200);

  let Module = null;
  let started = false;

  function setStatus(msg) {
    if (statusEl) statusEl.textContent = msg || "";
  }

  function pushKey(pressed, name) {
    if (!Module || !Module._I_PushKey) return;
    const code = KEY[name];
    if (code == null) return;
    Module._I_PushKey(pressed ? 1 : 0, code);
  }

  function bindPads() {
    document.querySelectorAll(".btn[data-key]").forEach((btn) => {
      const name = btn.getAttribute("data-key");
      const down = (e) => {
        e.preventDefault();
        btn.classList.add("is-down");
        pushKey(true, name);
      };
      const up = (e) => {
        e.preventDefault();
        btn.classList.remove("is-down");
        pushKey(false, name);
      };
      btn.addEventListener("pointerdown", down);
      btn.addEventListener("pointerup", up);
      btn.addEventListener("pointercancel", up);
      btn.addEventListener("pointerleave", up);
    });
  }

  function drawFrame() {
    if (!Module || !Module._I_GetFrameBuffer) return;
    const ptr = Module._I_GetFrameBuffer();
    if (!ptr) return;
    const src = Module.HEAPU8.subarray(ptr, ptr + 320 * 200 * 4);
    imageData.data.set(src);
    ctx.putImageData(imageData, 0, 0);
  }

  function lockOrientation() {
    const o = screen.orientation;
    if (o && o.lock) {
      o.lock("landscape").catch(() => {});
    }
  }

  async function boot() {
    if (started) return;
    started = true;
    startBtn.disabled = true;
    setStatus("Starting…");

    try {
      brand.hidden = true;
      stage.hidden = false;
      lockOrientation();
      Module = await createDoomModule({
        canvas,
        locateFile: (path) => path,
        onFrame: drawFrame,
        print: (t) => console.log(t),
        printErr: (t) => console.error(t),
      });
      setStatus("");
    } catch (err) {
      console.error(err);
      setStatus(String(err && err.message ? err.message : err));
      started = false;
      startBtn.disabled = false;
    }
  }

  // Prevent page scroll / bounce while playing
  document.addEventListener(
    "touchmove",
    (e) => {
      if (!stage.hidden) e.preventDefault();
    },
    { passive: false }
  );

  bindPads();
  startBtn.addEventListener("click", boot);

  // Preload wasm glue; show ready when script is present
  if (typeof createDoomModule === "function") {
    setStatus("Ready — tap Enter Hell");
  } else {
    setStatus("Missing doom.js build");
  }
})();
