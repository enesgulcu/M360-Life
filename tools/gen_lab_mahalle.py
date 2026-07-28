"""LabDuzZemin mahalle duvar + numarali tabela ureticisi.

5 genis alan (X), Z koridor ~2..48. Isik yok.
Tablo: M360_LabTabela_1..5 (yon tabelasi).
Yeniden uret: python tools/gen_lab_mahalle.py
"""
from pathlib import Path

BARRIER = (
    "{3E32CCC5E9979C47}Prefabs/Structures/Infrastructure/Barriers/"
    "CrashBarrier_E_01/CrashBarrier_E_01_6m.et"
)

# Sirali yon tabelalari (1..5) — fiziksel tabela + Hierarchy adi
TABELALAR = [
    (
        1,
        "{BE795E6CBF3D9AE5}Prefabs/Structures/Signs/Directions/"
        "SignDirection_Cain_01/SignDirection_02_Cain_01_1.et",
    ),
    (
        2,
        "{E091772F00B014EA}Prefabs/Structures/Signs/Directions/"
        "SignDirection_Cain_01/SignDirection_02_Cain_02_1.et",
    ),
    (
        3,
        "{149930B70D6D7C9E}Prefabs/Structures/Signs/Directions/"
        "SignDirection_Cain_01/SignDirection_02_Cain_03_1.et",
    ),
    (
        4,
        "{5D4125A87FAB08F4}Prefabs/Structures/Signs/Directions/"
        "SignDirection_Cain_01/SignDirection_02_Cain_04_1.et",
    ),
    (
        5,
        "{A949623072766080}Prefabs/Structures/Signs/Directions/"
        "SignDirection_Cain_01/SignDirection_02_Cain_05_1.et",
    ),
]

# Her alan 40 m; ayiricilar 40 / 80 / 120 / 160
ZONE_W = 40.0
ZONE_COUNT = 5
DIVIDER_XS = [ZONE_W * i for i in range(1, ZONE_COUNT)]  # 40,80,120,160

# Z derinlik: 2..48 (~46 m), bariyer her 6 m
Z_MIN, Z_MAX = 2.0, 48.0
Z_WALLS = list(range(5, 48, 6))  # 5,11,...,47
SIGN_Z = (Z_MIN + Z_MAX) / 2  # alan ortasi (Z)
# Sag kenarin biraz icinde; yuz -X (sola / alana bak)
SIGN_INSET = 1.5
SIGN_YAW = 90  # 0=-Z, 180=+Z (guneyden ice); 90=-X (sagdan sola)

# Pirinc: alan 2 (X 40-80), hizali Z=25
PIRINC = [
    ("M360_PirincTopla", "{ADF987310AA53059}Prefabs/M360/Isler/Pirinc/M360_Topla_Pirinc.et", 50.0, 25.0),
    ("M360_PirincIsle", "{ADF987310AA5305A}Prefabs/M360/Isler/Pirinc/M360_Isle_Pirinc.et", 60.0, 25.0),
    ("M360_PirincSat", "{ADF987310AA5305B}Prefabs/M360/Isler/Pirinc/M360_Sat_Pirinc.et", 70.0, 25.0),
]

SPAWN = (20.0, 0.5, 25.0)  # alan 1 merkez


def main() -> None:
    lines: list[str] = [
        "",
        "// --- M360 Lab mahalle ayiriclari (duvar + tabela 1-5) ---",
    ]

    # Numarali tabelalar: her alanin sag kenari, Z ortali, yuz sola (-X)
    for num, prefab in TABELALAR:
        x = ZONE_W * num - SIGN_INSET  # 38.5, 78.5, ...
        lines += [
            f'GenericEntity M360_LabTabela_{num} : "{prefab}" {{',
            f" coords {x:.1f} 0 {SIGN_Z:.1f}",
            f" angles 0 {SIGN_YAW} 0",
            "}",
        ]

    # Mahalle arasi duvarlar (X sabit, Z boyunca)
    for di, x in enumerate(DIVIDER_XS):
        tag = f"{di + 1}{di + 2}"  # 12, 23, 34, 45
        for i, z in enumerate(Z_WALLS):
            lines += [
                f'GenericEntity M360_LabDuvar_{tag}_{i + 1} : "{BARRIER}" {{',
                f" coords {x:.1f} 0 {z}",
                " angles 0 90 0",
                "}",
            ]

    # Kuzey / guney kenar (Z sabit, X boyunca) — hizali 6 m
    x_edge = list(range(3, int(ZONE_W * ZONE_COUNT), 6))  # 3..195
    for edge, z in (("N", Z_MIN), ("S", Z_MAX)):
        for i, x in enumerate(x_edge):
            lines += [
                f'GenericEntity M360_LabKenar_{edge}_{i + 1} : "{BARRIER}" {{',
                f" coords {x} 0 {z}",
                " angles 0 0 0",
                "}",
            ]

    snippet = "\n".join(lines) + "\n"
    layer_path = (
        Path(__file__).resolve().parents[1]
        / "m360-life/Worlds/LabDuzZemin/M360_LabDuzZemin_Layers/default.layer"
    )
    raw = layer_path.read_text(encoding="utf-8")
    if raw.startswith("\ufeff"):
        raw = raw.lstrip("\ufeff")

    marker = "// --- M360 Lab mahalle ayiriclari"
    if marker in raw:
        raw = raw[: raw.index(marker)].rstrip() + "\n"

    # Eski isik entity kalintisi (marker disinda olmamali ama guvenlik)
    # Spawn + pirinc konumlarini guncelle
    import re

    raw = re.sub(
        r'(SCR_SpawnPoint M360_PlayerSpawn : "[^"]+" \{\n) coords [^\n]+\n\}',
        rf"\1 coords {SPAWN[0]} {SPAWN[1]} {SPAWN[2]}\n}}",
        raw,
        count=1,
    )
    for name, prefab, x, z in PIRINC:
        raw = re.sub(
            rf'(GenericEntity {name} : "[^"]+" \{{\n) coords [^\n]+\n\}}',
            rf"\1 coords {x} 0.1 {z}\n}}",
            raw,
            count=1,
        )

    layer_path.write_text(raw.rstrip() + "\n" + snippet, encoding="utf-8", newline="\n")
    print(
        "ok",
        "tabela",
        ZONE_COUNT,
        "duvar_seg",
        snippet.count("M360_LabDuvar"),
        "kenar",
        snippet.count("M360_LabKenar"),
        "bytes",
        layer_path.stat().st_size,
    )


if __name__ == "__main__":
    main()
