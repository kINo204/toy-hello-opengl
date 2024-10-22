import numpy as np

times = 8
norm_scale = 100 * (4 ** times)

V = [
	[0, 0, 1],
	[0, 0.94, -0.33],
	[-0.82, -0.47, -0.33],

	[0, 0, 1],
	[0, 0.94, -0.33],
	[0.82, -0.47, -0.33],

	[0, 0, 1],
	[0.82, -0.47, -0.33],
	[-0.82, -0.47, -0.33],

	[0, 0.94, -0.33],
	[-0.82, -0.47, -0.33],
	[0.82, -0.47, -0.33],
]

Vt = []

def NORM(p):
	mod = (p[0]**2 + p[1]**2 + p[2]**2)**0.5
	res = []
	res.append(p[0] / mod)
	res.append(p[1] / mod)
	res.append(p[2] / mod)
	return res

def GENNORM(ps):
	[A, B, C] = ps
	AB = np.array(B) - np.array(A)
	AC = np.array(C) - np.array(A)
	norm = np.cross(AB, AC)
	if np.dot(np.array(A), norm) < 0:
		norm = - norm
	return norm * norm_scale

def format():
	with open('output.txt', 'w') as F:
		F.write(f"int len_ball = {len(V):};\n")
		F.write("int cfg_ball[] = {3,3};\n")
		F.write("float v_ball[] = {\n")
		for v in V:
			F.write(f"\t{v[0]:.6f}f, {v[1]:.6f}f, {v[2]:.6f}f, {v[3]:.6f}f, {v[4]:.6f}f, {v[5]:.6f}f,\n")
		F.write("};")

for i in range(0, times):
	# Num of faces.
	N = int(len(V) / 3)
	for i in range(0, N):
		i1 = 3 * i
		i2 = i1 + 1
		i3 = i2 + 1
		vs = [
			V[i1], V[i2], V[i3],
			NORM(np.array(V[i1])+np.array(V[i2])),
			NORM(np.array(V[i1])+np.array(V[i3])),
			NORM(np.array(V[i2])+np.array(V[i3])),
		]
		Vt.append(vs[0].copy())
		Vt.append(vs[3].copy())
		Vt.append(vs[4].copy())
		Vt.append(vs[1].copy())
		Vt.append(vs[3].copy())
		Vt.append(vs[5].copy())
		Vt.append(vs[2].copy())
		Vt.append(vs[4].copy())
		Vt.append(vs[5].copy())
		Vt.append(vs[3].copy())
		Vt.append(vs[4].copy())
		Vt.append(vs[5].copy())
	V = Vt
	Vt = []

N = int(len(V) / 3)
for i in range(0, N):
	i1 = 3 * i
	i2 = i1 + 1
	i3 = i2 + 1
	p = [V[i1], V[i2], V[i3]]
	norm = GENNORM(p)
	for a in range(0, 3):
		for b in range(0, 3):
			p[a].append(float(norm[b]))

format()
