@(
  set BASE=%~dp0
  for %%G in (*.asm) do (
    fasmg -e 5 -v 1 %%G "%BASE%..\..\image\amd64\%%~nG.efi"
  )
)
