Here is my binding.gyp file:
```gyp
{
  "targets": [
    {
      "target_name": "pickle_cpp",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "sources": [ "pickle_cpp.cc", "CppInterface.cpp",
                   "GameTimer/GameTimer.h", "GameTimer/GameTimer.cpp",
                   "TranslateConstant/TranslateConstant.h", "TranslateConstant/TranslateConstant.cpp",
                   "GameState/GameState.h", "GameState/GameState.cpp",
                   "RESET/RESET.h", "RESET/RESET.cpp",
                   "DIGI_V6_15/DIGI_V6_15.h",
                   "INPUTS/INPUTS.h", "INPUTS/INPUTS.cpp",
                   "GAME_MODES/GAME_MODES.h", "GAME_MODES/GAME_MODES.cpp",
                   "ScoreBoard/ScoreBoard.h", "ScoreBoard/ScoreBoard.cpp",
                   "GameObject/GameObject.h", "GameObject/GameObject.cpp",
                   "PinState/PinState.h", "PinState/PinState.cpp",
                   "PinInterface/PinInterface.h", "PinInterface/PinInterface.cpp",
                   "Player/Player.h", "Player/Player.cpp",
                   "Serial.h", "Serial.cpp",
                   "Arduino/Arduino.h",
                   "Team/Team.h", "Team/Team.cpp",
                   "Rules/Rules.h", "Rules/Rules.cpp",
                   "ScoreCommand/ScoreCommand.h", "ScoreCommand/ScoreCommand.cpp",
                   "Command/Command.h",
                   "Logger/Logger.h", "Logger/Logger.cpp",
                   "History/History.h", "History/History.cpp",
                   "GameStateManager/GameStateManager.h", "GameStateManager/GameStateManager.cpp",
                   "RemoteCodeTranslator/RemoteCodeTranslator.h", "RemoteCodeTranslator/RemoteCodeTranslator.cpp",
                   "RemoteLocker/RemoteLocker.h", "RemoteLocker/RemoteLocker.cpp",
                   "ShowMatchWin/ShowMatchWin.h", "ShowMatchWin/ShowMatchWin.cpp",
                   "ZeroCondition/ZeroCondition.h", "ZeroCondition/ZeroCondition.cpp",
                   "Observer/Observer.h", "Subject/Subject.h"
                    ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "UNDO", "DIGI_V6_15", "GAME_LEDs", "GAME_MODES", "Arduino", "PinState",
        "PinInterface", "Player", "GameState", "GameTimer", "INPUTS", "RESET",
        "POINT_LEDs", "MODE_1_FUNCTIONS", "MODE_1_SCORE", "SERVE_LEDs", "MODE_1_TIEBREAKER",
        "MODE_1_WIN_SEQUENCES", "SET_LEDs", "MODE_2_FUNCTIONS", "GameObject",
        "ScoreBoard", "TranslateConstant", "Team", "Rules", "ScoreCommand",
        "Command", "Logger", "History", "GameStateManager", "RemoteCodeTranslator",
        "RemoteLocker", "ShowMatchWin", "ZeroCondition", "Observer", "Subject"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
    }
  ]
}
```

Here is the output: 
```bash
> object_wrap@0.0.0 install
> node-gyp rebuild

gyp info it worked if it ends with ok
gyp info using node-gyp@9.3.1
gyp info using node@18.7.0 | win32 | x64
gyp info find Python using Python version 3.9.13 found at "C:\Users\EG\AppData\Local\Microsoft\WindowsApps\PythonSoftwareFoundation.Python.3.9_qbz5n2kfra8p0\python.exe"
gyp info find VS using VS2019 (16.10.31515.178) found at:
gyp info find VS "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community"
gyp info find VS run with --verbose for detailed information
gyp info spawn C:\Users\EG\AppData\Local\Microsoft\WindowsApps\PythonSoftwareFoundation.Python.3.9_qbz5n2kfra8p0\python.exe
gyp info spawn args [
gyp info spawn args   'C:\\Users\\EG\\AppData\\Roaming\\npm\\node_modules\\npm\\node_modules\\node-gyp\\gyp\\gyp_main.py',
gyp info spawn args   'binding.gyp',
gyp info spawn args   '-f',
gyp info spawn args   'msvs',
gyp info spawn args   '-I',
gyp info spawn args   'C:\\Users\\EG\\Desktop\\2022\\may\\2nd_week\\pickle_cpp\\build\\config.gypi',
gyp info spawn args   '-I',
gyp info spawn args   'C:\\Users\\EG\\AppData\\Roaming\\npm\\node_modules\\npm\\node_modules\\node-gyp\\addon.gypi',
gyp info spawn args   '-I',
gyp info spawn args   'C:\\Users\\EG\\AppData\\Local\\node-gyp\\Cache\\18.7.0\\include\\node\\common.gypi',
gyp info spawn args   '-Dlibrary=shared_library',
gyp info spawn args   '-Dvisibility=default',
gyp info spawn args   '-Dnode_root_dir=C:\\Users\\EG\\AppData\\Local\\node-gyp\\Cache\\18.7.0',
gyp info spawn args   '-Dnode_gyp_dir=C:\\Users\\EG\\AppData\\Roaming\\npm\\node_modules\\npm\\node_modules\\node-gyp',
gyp info spawn args   '-Dnode_lib_file=C:\\\\Users\\\\EG\\\\AppData\\\\Local\\\\node-gyp\\\\Cache\\\\18.7.0\\\\<(target_arch)\\\\node.lib',
gyp info spawn args   '-Dmodule_root_dir=C:\\Users\\EG\\Desktop\\2022\\may\\2nd_week\\pickle_cpp',
gyp info spawn args   '-Dnode_engine=v8',
gyp info spawn args   '--depth=.',
gyp info spawn args   '--no-parallel',
gyp info spawn args   '--generator-output',
gyp info spawn args   'C:\\Users\\EG\\Desktop\\2022\\may\\2nd_week\\pickle_cpp\\build',
gyp info spawn args   '-Goutput_dir=.'
gyp info spawn args ]
gyp info spawn C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe
gyp info spawn args [
gyp info spawn args   'build/binding.sln',
gyp info spawn args   '/clp:Verbosity=minimal',
gyp info spawn args   '/nologo',
gyp info spawn args   '/p:Configuration=Release;Platform=x64'
gyp info spawn args ]
Building the projects in this solution one at a time. To enable parallel build, please add the "-m" switch.
  pickle_cpp.cc
  CppInterface.cpp
  GameTimer.cpp
  TranslateConstant.cpp
  GameState.cpp
  RESET.cpp
  INPUTS.cpp
  GAME_MODES.cpp
  ScoreBoard.cpp
  GameObject.cpp
  PinState.cpp
  PinInterface.cpp
  Player.cpp
  Serial.cpp
  Team.cpp
  Rules.cpp
  ScoreCommand.cpp
  Logger.cpp
  History.cpp
  GameStateManager.cpp
  RemoteCodeTranslator.cpp
  RemoteLocker.cpp
  ShowMatchWin.cpp
  ZeroCondition.cpp
  win_delay_load_hook.cc
  more output...
```

How do I add the -m switch to make it faster?