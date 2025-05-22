# Giải thích các hàm trong Cube và Sphere

## Cube (Hình lập phương)

### Các biến toàn cục
- `NumPoints`: Số lượng điểm cần thiết để vẽ hình lập phương (36 điểm)
- `points[]`: Mảng chứa tọa độ các điểm của hình lập phương
- `colors[]`: Mảng chứa màu sắc của các điểm
- `normals[]`: Mảng chứa vector pháp tuyến của các điểm
- `vertices[]`: Mảng chứa 8 đỉnh của hình lập phương
- `vertex_colors[]`: Mảng chứa màu sắc của 8 đỉnh
- `vao_cube`, `vbo_cube`: Các buffer object cho OpenGL

### Các hàm chính

#### `initCube()`
- Khởi tạo tọa độ cho 8 đỉnh của hình lập phương
- Gán màu sắc cho từng đỉnh
- In ra thông tin về VAO và VBO

#### `quad(int a, int b, int c, int d)`
- Tạo một mặt của hình lập phương từ 4 đỉnh
- Tính toán vector pháp tuyến cho mặt
- Thêm các điểm, màu sắc và vector pháp tuyến vào mảng tương ứng

#### `makeColorCube()`
- Tạo 6 mặt của hình lập phương bằng cách gọi hàm `quad()`
- Mỗi mặt được tạo từ 4 đỉnh

#### `generateGeometry()`
- Gọi `initCube()` để khởi tạo dữ liệu
- Gọi `makeColorCube()` để tạo hình lập phương

#### `initGPUBuffers()`
- Tạo và cấu hình VAO (Vertex Array Object)
- Tạo và cấu hình VBO (Vertex Buffer Object)
- Thiết lập các thuộc tính cho shader (vị trí, màu sắc, vector pháp tuyến)

#### `drawCubeWithLighting(GLuint program, GLuint model_loc, mat4 model)`
- Vẽ hình lập phương với hiệu ứng ánh sáng
- Thiết lập các tham số chiếu sáng (vị trí ánh sáng, màu ambient, diffuse, specular)
- Thiết lập các thuộc tính vật liệu
- Áp dụng ma trận biến đổi model
- Vẽ hình lập phương

## Sphere (Hình cầu)

### Các biến toàn cục
- `SphereStacks`, `SphereSlices`: Số lượng lát cắt theo chiều dọc và ngang
- `SpherePoints`: Tổng số điểm cần thiết để vẽ hình cầu
- `spherePoints[]`: Mảng chứa tọa độ các điểm của hình cầu
- `sphereColors[]`: Mảng chứa màu sắc của các điểm
- `sphereNormals[]`: Mảng chứa vector pháp tuyến của các điểm
- `vao_sphere`, `vbo_sphere`: Các buffer object cho OpenGL

### Các hàm chính

#### `initSphereGeometry()`
- Tạo dữ liệu cho hình cầu bằng cách chia thành các lát cắt
- Tính toán tọa độ các điểm trên bề mặt hình cầu
- Tạo các tam giác từ các điểm để tạo bề mặt hình cầu
- Thiết lập màu sắc và vector pháp tuyến cho từng điểm
- Tạo và cấu hình VAO và VBO
- Thiết lập các thuộc tính cho shader

#### `drawSphereWithLighting(GLuint program, GLuint model_loc, mat4 model)`
- Vẽ hình cầu với hiệu ứng ánh sáng
- Thiết lập các tham số chiếu sáng (vị trí ánh sáng, màu ambient, diffuse, specular)
- Thiết lập các thuộc tính vật liệu
- Áp dụng ma trận biến đổi model
- Vẽ hình cầu 

## Cách sử dụng trong hàm main

### Khởi tạo
1. Trong hàm `init()`:
   ```cpp
   // Khởi tạo shader program
   GLuint program = InitShader("vshader.glsl", "fshader.glsl");
   glUseProgram(program);

   // Khởi tạo hình lập phương
   generateGeometry();
   initGPUBuffers();

   // Khởi tạo hình cầu
   initSphereGeometry();

   // Lấy vị trí của biến uniform trong shader
   GLuint model_loc = glGetUniformLocation(program, "Model");
   ```

### Vẽ hình
1. Trong hàm `display()`:
   ```cpp
   // Xóa buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Tạo ma trận biến đổi model
   mat4 model = mat4(1.0); // Ma trận đơn vị

   // Vẽ hình lập phương
   model = Translate(x, y, z) * Rotate(angle, 0.0, 1.0, 0.0) * Scale(scale, scale, scale);
   drawCubeWithLighting(program, model_loc, model);

   // Vẽ hình cầu
   model = Translate(x2, y2, z2) * Rotate(angle2, 0.0, 1.0, 0.0) * Scale(scale2, scale2, scale2);
   drawSphereWithLighting(program, model_loc, model);

   glutSwapBuffers();
   ```

### Cập nhật animation
1. Trong hàm `update()`:
   ```cpp
   // Cập nhật góc xoay
   angle += 2.0;
   angle2 += 1.5;
   if (angle > 360.0) angle -= 360.0;
   if (angle2 > 360.0) angle2 -= 360.0;

   glutPostRedisplay();
   ```

### Xử lý input
1. Trong hàm `keyboard()`:
   ```cpp
   switch (key) {
   case 'q': case 'Q':
       exit(EXIT_SUCCESS);
       break;
   case ' ': // Phím space
       // Xử lý animation
       break;
   }
   ```

