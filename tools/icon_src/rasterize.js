const fs = require("fs");
const path = require("path");
const { Resvg } = require("@resvg/resvg-js");

const dir = __dirname;
const outDir = path.join(dir, "png");
fs.mkdirSync(outDir, { recursive: true });

const names = ["heart", "food", "cup-water", "cash", "wallet", "plus"];

for (const name of names) {
  let svg = fs.readFileSync(path.join(dir, `${name}.svg`), "utf8");
  // Ensure transparent background (no black rect)
  if (!svg.includes('xmlns="http://www.w3.org/2000/svg"')) {
    svg = svg.replace("<svg", '<svg xmlns="http://www.w3.org/2000/svg"');
  }
  const resvg = new Resvg(svg, {
    fitTo: { mode: "width", value: 256 },
    background: "rgba(0,0,0,0)",
  });
  const pngData = resvg.render().asPng();
  const out = path.join(outDir, `${name}.png`);
  fs.writeFileSync(out, pngData);
  console.log("wrote", out, pngData.length);
}
