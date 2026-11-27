# Gera src/frontend/assets/dragao_mascara.hpp a partir de src/frontend/assets/dragao.png (o dragão do NidraIA).
# A máscara guarda só a forma (o dourado, sem o fundo e sem os detalhes roxos de olho e boca), em 96 x 96;
# o NiGBA pinta com a cor do tema. Rodar de novo só se o desenho mudar:
#   powershell -ExecutionPolicy Bypass -File tools\gerar-dragao.ps1
Add-Type -AssemblyName System.Drawing
$raiz = Split-Path -Parent $PSScriptRoot
$origem = Join-Path $raiz 'src\frontend\assets\dragao.png'
$destino = Join-Path $raiz 'src\frontend\assets\dragao_mascara.hpp'
$lado = 96

$png = [System.Drawing.Bitmap]::FromFile($origem)
# 1) Silhueta em tamanho cheio: branco, com alfa = transparência original x "quanto é dourado" (vermelho acima do azul).
$forma = New-Object System.Drawing.Bitmap $png.Width, $png.Height, ([System.Drawing.Imaging.PixelFormat]::Format32bppArgb)
for ($y = 0; $y -lt $png.Height; $y++) {
    for ($x = 0; $x -lt $png.Width; $x++) {
        $c = $png.GetPixel($x, $y)
        $dourado = [Math]::Min(1.0, [Math]::Max(0.0, ($c.R - $c.B - 40) / 60.0))
        $a = [int][Math]::Round($c.A * $dourado)
        $forma.SetPixel($x, $y, [System.Drawing.Color]::FromArgb($a, 255, 255, 255))
    }
}
# 2) Reduz para lado x lado, centralizado, mantendo a proporção.
$mini = New-Object System.Drawing.Bitmap $lado, $lado, ([System.Drawing.Imaging.PixelFormat]::Format32bppArgb)
$g = [System.Drawing.Graphics]::FromImage($mini)
$g.InterpolationMode = [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
$g.PixelOffsetMode = [System.Drawing.Drawing2D.PixelOffsetMode]::HighQuality
$escala = [Math]::Min($lado / $png.Width, $lado / $png.Height)
$w = $png.Width * $escala; $h = $png.Height * $escala
$g.DrawImage($forma, [single](($lado - $w) / 2), [single](($lado - $h) / 2), [single]$w, [single]$h)
$g.Dispose()

$linhas = New-Object System.Collections.Generic.List[string]
for ($y = 0; $y -lt $lado; $y++) {
    $valores = for ($x = 0; $x -lt $lado; $x++) { '{0,3}' -f $mini.GetPixel($x, $y).A }
    $linhas.Add('    ' + ($valores -join ',') + ',')
}
$texto = @"
#pragma once
// Gerado por tools/gerar-dragao.ps1 a partir de dragao.png (o dragão do NidraIA). Não editar à mão.
// Silhueta do dragão: $lado x $lado valores de alfa (0 = vazio, 255 = cheio), linha por linha.

#include <cstdint>

constexpr int kDragaoLado = $lado;
constexpr uint8_t kDragaoMascara[kDragaoLado * kDragaoLado] = {
$($linhas -join "`n")
};
"@
[System.IO.File]::WriteAllText($destino, $texto.Replace("`r`n", "`n") + "`n", (New-Object System.Text.UTF8Encoding $false))
$png.Dispose(); $forma.Dispose(); $mini.Dispose()
Write-Output "Gerado: $destino"
