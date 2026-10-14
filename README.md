# README

[toc]

## Problem Statement

1. 问题描述如下：

    .\scripts\build.ps1: 术语 '.\scripts\build.ps1' 不会被识别为 cmdlet、函数、脚本文件或可执行程序的名称。
    请检查名称的拼写或验证路径是否正确(如果包含路径)，然后重试。

解决方法：

    这个错误表示 PowerShell 当前目录下找不到 .\scripts\build.ps1，脚本本身还没有开始运行。先切换到项目目录，再运行：

    ```powershell
    Set-Location 'D:\Felands\Felands\Project\Render\Vulkan\Demo'
    .\scripts\build.ps1
    ```