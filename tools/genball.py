import numpy as np

def midpoint(p1, p2):
    """计算两个点的中点"""
    return (p1 + p2) / 2

def subdivide_face(vertices, depth):
    """递归细分一个面"""
    if depth == 0:
        return vertices
    else:
        # 计算三个新顶点，它们是原顶点的中点
        mid1 = midpoint(vertices[0], vertices[1])
        mid2 = midpoint(vertices[1], vertices[2])
        mid3 = midpoint(vertices[2], vertices[0])
        
        # 递归细分新的三角形
        new_vertices = subdivide_face([mid1, mid2, mid3], depth-1)
        new_vertices.extend(subdivide_face([mid1, vertices[0], mid3], depth-1))
        new_vertices.extend(subdivide_face([mid2, mid1, vertices[1]], depth-1))
        new_vertices.extend(subdivide_face([mid3, mid2, vertices[2]], depth-1))
        return new_vertices

def subdivide_tetrahedron(vertices, depth):
    """递归细分四面体"""
    if depth == 0:
        return vertices
    else:
        # 计算四面体的6条边的中点
        mid_points = [midpoint(vertices[i], vertices[(i+1)%4]) for i in range(4)]
        # 计算四面体的4个面的中心点
        center_points = [midpoint(mid_points[i], mid_points[(i+2)%4]) for i in range(4)]
        
        # 递归细分每个更小的四面体
        new_vertices = []
        for i in range(4):
            sub_tetra = [vertices[i], mid_points[i], center_points[i], mid_points[(i+1)%4]]
            new_vertices.extend(subdivide_tetrahedron(sub_tetra, depth-1))
        
        return new_vertices

# 定义单位四面体的顶点
tetrahedron_vertices = np.array([
    [1, 1, 1],  # 顶点A
    [1, -1, -1], # 顶点B
    [-1, 1, -1], # 顶点C
    [-1, -1, 1]  # 顶点D
])

# 递归细分四面体
subdivided_vertices = subdivide_tetrahedron(tetrahedron_vertices, 2)

# 归一化顶点坐标到单位范围
normalized_vertices = np.linalg.norm(subdivided_vertices, axis=1)
unit_vertices = subdivided_vertices / np.max(normalized_vertices)

# 将顶点组织成三角形
triangles = []
for i in range(len(unit_vertices) - 2):
    triangles.append([unit_vertices[i], unit_vertices[i+1], unit_vertices[i+2]])

print("len_ball = {:};".format(len(triangles) * 3))
print("cfg_ball = { 3 };")
print("v_ball = {")
for triangle in triangles:
    for i in range(0, 3):
        print("\t{:.4f}f,".format(triangle[i][0]),end='')
        print("{:.4f}f,".format(triangle[i][1]),end='')
        print("{:.4f}f,".format(triangle[i][2]))
print("};")
