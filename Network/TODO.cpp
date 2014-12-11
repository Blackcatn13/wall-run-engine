/*
header 0x55ff
#materials

	|
	|	Vertex Type
M	|   |
	| N | Texture size
	|	| Texture Name
	|	|

	|	# vertex
M	|	vertex
	|	#idx
	|	idx

footer 0xff55

// M # materials, N # textures for material

exportMesh Obj

	file
	save header
	save #material
	for
		exportMaterial material file
		#vtx
		#idx
		GenerateVertex vtx idx
		for
			ExportVertex vtx[i]
			ExportIdx	idx[i]
	save footer

GenerateVertex vtx idx
	for Faces
		vtx1 ...
		TotVertex1
		id1 = AddVertex TotVertex


*/

/*

set path -> directori exe



numtriangles
numvertex
numdrawprimitives


*/