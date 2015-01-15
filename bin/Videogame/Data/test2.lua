-- Ojo!!! Las clases tiene que tener constructor si o si?

local name = "Teta"
local n1 = CNamed();
n1:set_name(name);

local point3 = Vect3f(0,0,0);

local m_ObjectThPS = CObject3D(Vect3f(0, 0, 0), 0, 0, 0);
local m_ThPSCamera = CThPSCamera(0.1, 100.0, (45.0 * 3.14 / 180.0), 1.0, m_ObjectThPS, 50);

local cam_Controller = CCameraController();
cam_Controller:add_new_camera("luaCam", m_ThPSCamera);
--cam_Controller:update(1.0);

--local lightM1 = CLightManager();
--lightM1:load("./Data/file.xml");