#!/usr/bin/env python3
"""Generate Circle HUD textures from downloaded MDI icons (thick rings, large icons)."""
from __future__ import annotations

import math
import os
import struct
from PIL import Image

ROOT = os.path.join(os.path.dirname(__file__), "..")
OUT = os.path.join(ROOT, "UI", "Textures", "M360")
ICON_DIR = os.path.join(os.path.dirname(__file__), "icon_src", "png")
SIZE = 256
CX = CY = SIZE // 2


def write_dds(path: str, img: Image.Image) -> None:
    img = img.convert("RGBA").resize((SIZE, SIZE), Image.Resampling.LANCZOS)
    header = bytearray(128)
    header[0:4] = b"DDS "
    struct.pack_into("<I", header, 4, 124)
    struct.pack_into("<I", header, 8, 0x00081007)
    struct.pack_into("<I", header, 12, SIZE)
    struct.pack_into("<I", header, 16, SIZE)
    struct.pack_into("<I", header, 20, SIZE * SIZE * 4)
    struct.pack_into("<I", header, 24, 0)
    struct.pack_into("<I", header, 28, 0)
    struct.pack_into("<I", header, 76, 32)
    struct.pack_into("<I", header, 80, 0x41)
    struct.pack_into("<I", header, 84, 0)
    struct.pack_into("<I", header, 88, 32)
    struct.pack_into("<I", header, 92, 0x00FF0000)
    struct.pack_into("<I", header, 96, 0x0000FF00)
    struct.pack_into("<I", header, 100, 0x000000FF)
    struct.pack_into("<I", header, 104, 0xFF000000)
    struct.pack_into("<I", header, 108, 0x1000)

    pixels = bytearray()
    for y in range(SIZE):
        for x in range(SIZE):
            r, g, b, a = img.getpixel((x, y))
            pixels.extend((b, g, r, a))
    with open(path, "wb") as f:
        f.write(header)
        f.write(pixels)


def save(name: str, img: Image.Image) -> None:
    os.makedirs(OUT, exist_ok=True)
    img = img.convert("RGBA")
    img.save(os.path.join(OUT, f"{name}.png"), "PNG")
    write_dds(os.path.join(OUT, f"{name}.edds"), img)
    print("wrote", name)


def blank() -> Image.Image:
    return Image.new("RGBA", (SIZE, SIZE), (0, 0, 0, 0))


def disc(radius: float, color: tuple[int, int, int, int], soft: float = 1.5) -> Image.Image:
    img = blank()
    px = img.load()
    for y in range(SIZE):
        for x in range(SIZE):
            dx = x + 0.5 - CX
            dy = y + 0.5 - CY
            d = math.sqrt(dx * dx + dy * dy)
            if d <= radius:
                px[x, y] = color
            elif d < radius + soft:
                t = 1.0 - (d - radius) / soft
                px[x, y] = (color[0], color[1], color[2], int(color[3] * t))
    return img


def ring(r_outer: float, r_inner: float, color: tuple[int, int, int, int], soft: float = 1.2) -> Image.Image:
    img = blank()
    px = img.load()
    for y in range(SIZE):
        for x in range(SIZE):
            dx = x + 0.5 - CX
            dy = y + 0.5 - CY
            d = math.sqrt(dx * dx + dy * dy)
            if r_inner - soft <= d <= r_outer + soft:
                if d < r_inner:
                    t = 1.0 - (r_inner - d) / soft
                elif d > r_outer:
                    t = 1.0 - (d - r_outer) / soft
                else:
                    t = 1.0
                a = int(color[3] * max(0.0, min(1.0, t)))
                if a > 0:
                    px[x, y] = (color[0], color[1], color[2], a)
    return img


def glow_ring(r: float, thickness: float, color: tuple[int, int, int], strength: float = 0.55) -> Image.Image:
    img = blank()
    px = img.load()
    for y in range(SIZE):
        for x in range(SIZE):
            dx = x + 0.5 - CX
            dy = y + 0.5 - CY
            d = math.sqrt(dx * dx + dy * dy)
            dist = abs(d - r)
            if dist < thickness * 2.5:
                t = math.exp(-(dist * dist) / (2 * (thickness * 0.7) ** 2))
                a = int(255 * strength * t)
                if a > 0:
                    px[x, y] = (color[0], color[1], color[2], a)
    return img


def load_icon(name: str) -> Image.Image:
    path = os.path.join(ICON_DIR, f"{name}.png")
    im = Image.open(path).convert("RGBA")
    # force RGB white, keep alpha
    out = blank()
    src = im.load()
    dst = out.load()
    for y in range(SIZE):
        for x in range(SIZE):
            r, g, b, a = src[x, y]
            if a > 0:
                dst[x, y] = (255, 255, 255, a)
    return out


def scale_icon(icon: Image.Image, factor: float) -> Image.Image:
    w = max(8, int(SIZE * factor))
    resized = icon.resize((w, w), Image.Resampling.LANCZOS)
    canvas = blank()
    ox = (SIZE - w) // 2
    oy = (SIZE - w) // 2
    canvas.paste(resized, (ox, oy), resized)
    return canvas


def make_badge(icon_name: str, glow_rgb: tuple[int, int, int], icon_scale: float = 0.42) -> Image.Image:
    # Orta kalinlik halka (~16px) + icerde paddingli ikon
    bg = disc(92, (8, 10, 14, 240), soft=2.0)
    track = ring(114, 98, (48, 52, 60, 200), soft=1.2)
    g = glow_ring(106, 10, glow_rgb, strength=0.22)
    ic = scale_icon(load_icon(icon_name), icon_scale)
    out = bg.copy()
    out.alpha_composite(g)
    out.alpha_composite(track)
    out.alpha_composite(ic)
    return out


def make_flat_icon(icon_name: str, pad_factor: float = 0.78) -> Image.Image:
    """Standalone white icon on transparent (money / plus)."""
    return scale_icon(load_icon(icon_name), pad_factor)


def main() -> None:
    save("m360_can_UI", make_flat_icon("heart", 0.80))
    save("m360_yemek_UI", make_flat_icon("food", 0.80))
    save("m360_su_UI", make_flat_icon("cup-water", 0.80))
    save("m360_nakit_UI", make_flat_icon("wallet", 0.78))
    save("m360_plus_UI", make_flat_icon("plus", 0.82))

    save("m360_badge_can_UI", make_badge("heart", (255, 55, 140), 0.42))
    save("m360_badge_yemek_UI", make_badge("food", (255, 170, 60), 0.40))
    save("m360_badge_su_UI", make_badge("cup-water", (50, 230, 240), 0.40))

    # Orta kalinlik progress ring (~16px)
    save("m360_radial_ring_UI", ring(114, 98, (255, 255, 255, 255), soft=1.2))
    save("m360_radial_mask_UI", disc(118, (255, 255, 255, 255), soft=1.0))
    save("m360_radial_bg_UI", disc(92, (12, 14, 17, 235), soft=2.0))
    save("m360_radial_glow_UI", glow_ring(106, 12, (255, 255, 255), strength=0.28))
    print("done")


if __name__ == "__main__":
    main()
