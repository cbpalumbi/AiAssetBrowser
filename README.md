In progress. Currently can load and preview assets and assign/modify their tags. Each asset has a .glb, a .qml, a .history.json, and a .metadata.json. <br><br>
3D viewport supports rotations and zoom. The viewport is actually an embedded Qt Quick 3D scene, while the rest of the app is built in Qt Widgets. Models from Sketchfab - in order to render with Qt Quick 3D, need to preprocess the glbs with balsam.exe, incl. with Qt. <br><br>
See PythonScripts for some early tag prediction. Not integrated into C++ app yet.




https://github.com/user-attachments/assets/8e6feabe-3fc7-4f56-8995-4ce50695da09


