param(
    [string]$BuildDir = "build-cov"
)

$Root = Resolve-Path (Join-Path $PSScriptRoot "..")
$ObjectDir = Join-Path $Root $BuildDir

Push-Location $ObjectDir
& (Join-Path $ObjectDir "test/online_shopping_tests.exe")
Pop-Location

Write-Host "`n=== Overall line coverage ===" -ForegroundColor Cyan
gcovr -r $Root --object-directory $ObjectDir `
    --filter "Server/Headers" --filter "Client/Headers" `
    --txt

Write-Host "`n=== Overall branch coverage ===" -ForegroundColor Cyan
gcovr -r $Root --object-directory $ObjectDir `
    --filter "Server/Headers" --filter "Client/Headers" `
    --txt-metric branch

Write-Host "`n=== Module-wise summary (line + branch) ===" -ForegroundColor Cyan
gcovr -r $Root --object-directory $ObjectDir `
    --filter "Server/Headers" --filter "Client/Headers" `
    --csv --csv-column-filename --csv-column-line --csv-column-branch --csv-column-taken `
    | Select-Object -Skip 1

$htmlDir = Join-Path $ObjectDir "coverage-html"
gcovr -r $Root --object-directory $ObjectDir `
    --filter "Server/Headers" --filter "Client/Headers" `
    --html --html-details -o $htmlDir
Write-Host "HTML report: $htmlDir/index.html" -ForegroundColor Green
