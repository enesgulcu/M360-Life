# LabDuzZemin — lab harita (geliştirme zemini)

Düz zemin. Sistemler burada blok/mahalle olarak kurulur; Everon yerleştirme **en sonda**.

## Mahalleler (X ekseni, her biri 40 m; Z ≈ 2–48)

| No | Kod | X aralığı | Merkez | Amaç |
|---|---|---|---|---|
| **1** | Hub | 0–40 | 20 | Spawn (20,25), HUD/API |
| **2** | İşler | 40–80 | 60 | Pirinç (50 / 60 / 70, Z25) |
| **3** | Ekonomi | 80–120 | 100 | ATM / market (sonra) |
| **4** | Araç | 120–160 | 140 | Garaj / park (sonra) |
| **5** | Rol | 160–200 | 180 | Polis / doktor / klan (sonra) |

Ayırıcılar: `CrashBarrier_E_01_6m` (mahalle arası X=40/80/120/160 + kuzey/güney kenar).  
Tabelalar: `M360_LabTabela_1` … `_5` (her alanın sağ kenarı, Z ortalı, yüz sola).

Yeniden üret: `python tools/gen_lab_mahalle.py`  
Workbench: dünyayı yeniden aç / Play.
