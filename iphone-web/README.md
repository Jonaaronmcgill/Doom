# DOOM on iPhone (Safari / WebAssembly)

Runs the **linuxdoom-1.10** engine from this repo in Mobile Safari on an
iPhone 16 Pro Max (and other phones) via WebAssembly.

Native App Store / Xcode builds are not produced here — this cloud environment
is Linux. Safari is the practical way to play on the phone from this port.

## What you get

- Original game logic from `linuxdoom-1.10`
- New platform layer under `iphone-web/src` (canvas video, stub sound/net)
- Touch controls sized for iPhone 16 Pro Max (safe areas, Dynamic Island)
- Bundled [Freedoom](https://freedoom.github.io/) Phase 1 IWAD as `doom1.wad`

## Build

```bash
source ~/emsdk/emsdk_env.sh   # or install emsdk first
cd iphone-web
make
```

Output is in `dist/` (`index.html`, `doom.js`, `doom.wasm`, `doom.data`).

## Play on iPhone 16 Pro Max

1. Serve `dist/` over HTTPS (or localhost via a tunnel).
2. Open the URL in Safari.
3. Add to Home Screen for a fullscreen feel.
4. Tap **Enter Hell**, rotate to landscape, use on-screen controls.

### Controls

| Control | Action |
|--------|--------|
| D-pad | Move / turn |
| FIRE | Shoot (Ctrl) |
| USE | Open doors / switches (Space) |
| STR / RUN | Strafe modifier / run |
| ESC / ENT / MAP | Menu, confirm, automap |
| 1–3 | Weapons |

## Notes

- Sound is currently stubbed (silent).
- Real commercial Doom WADs work if you replace `wad/doom1.wad` and rebuild
  (you must own the data).
- For a native UIKit/Metal app you would still need a Mac + Xcode; this web
  port is what runs on the phone from this repository today.
