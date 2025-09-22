@echo off
@REM stop all electron processes
powershell.exe -Command "Get-Process -Name 'electron' | Stop-Process"

@REM run "npm install"
npm install

Add-Type -AssemblyName System.Windows.Forms

$form = New-Object Windows.Forms.Form
$form.Text = "My Form"
$form.Size = New-Object Drawing.Size @(200,100)
$form.StartPosition = "CenterScreen"

$button = New-Object Windows.Forms.Button
$button.Text = "Click me!"
$button.Size = New-Object Drawing.Size @(100,30)
$button.Location = New-Object Drawing.Point @(50,20)
$button.Add_Click({[System.Windows.Forms.MessageBox]::Show("Hello")})

$form.Controls.Add($button)

$form.ShowDialog()


