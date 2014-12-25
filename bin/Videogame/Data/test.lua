local v1 = Vect3f(1, 0, 0);
local v2 = Vect3f(1, 1, 1);
local v3 = Vect3f(v1 + v2);

local o1 = CObject3D();
o1.SetYaw(0.1);
o1.SetRoll(0.2);
o1.SetPitch(0.3);

local n1 = CName();
n1.SetName("Tete");
n1.GetMame();

local vs1 = CVisible();
vs1.SetVisible(true);
vs1.GetVisible();