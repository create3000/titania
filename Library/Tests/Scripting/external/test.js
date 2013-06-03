
function set_time (value)
{
	var scene = Browser .createX3DFromURL (new MFString ('../Cube.wrl'));
	
	Browser .replaceWorld (scene);
}
