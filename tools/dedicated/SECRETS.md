# Sirlar — GIT'E GIRMEZ
#
# Tek kaynak (onerilen):
#   api/.env
#     DATABASE_URL=...          (Neon)
#     M360_SERVER_KEY=...       (oyun + API ayni anahtar)
#
# pc-hazirla / start.ps1 bunu okur -> tools/dedicated/secrets/M360_ApiLabKey.txt
# start.ps1 ayrica oyun profiline kopyalar ($profile:M360_ApiLabKey.txt)
#
# api/.env her PC'de bir kez olmali (USB / sifre yoneticisi).
# Ornek: api/.env.example
#
# Diger dosyalar (otomatik / istege bagli):
#   secrets/server-root.txt   — bu PC'deki Server yolu
#   secrets/server.json       — sablon
