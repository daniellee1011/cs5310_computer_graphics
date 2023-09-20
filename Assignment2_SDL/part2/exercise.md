# exercise.md

The following are a list of questions about the design of our math library and also further your C++/general programming knowledge.

1. In C++ what are the three instances where you would use the keyword [const](https://en.cppreference.com/w/cpp/keyword/const)?
2. What does [inline](https://en.cppreference.com/w/cpp/language/inline) do? Why might a programmer use it?
3. Why do you think libraries like GLM are implemented as a header only library? 
	- (Sometimes you will see this, and you may research [.inl files](https://stackoverflow.com/questions/1208028/significance-of-a-inl-file-in-c) in C++)
4. C++ allows for something known as 'operator overloading'. This means we can define (or redefine) how operators like +,-,/,etc. work--but this means additional symbols like `<<`. Would it be helpful to implement the '<<' operator for a mat4 type? (i.e. `operator<<`)? What might you use this for? Does the GLM library overload the '<<' operator? (i.e., `std::cout << some_mat4`)
5. For the floating-point data type, would it be dangerous to overload the operator for '=='?
	- How might you mitigate this? [reference](https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/)
	- The question I am really asking is, why is it a problem to compare two floating point numbers?
6. What is a template in C++? Is the GLM library using templates?
7. Do a little research on your own for this one. How closely named are the functions in GLM versus the GL Shading Language(GLSL) (i.e., Was the GLM library designed to closely match GLSL?)
8. Do you feel comfortable working with pointers? (Yes/No) -- If not -- here's a video: https://www.youtube.com/watch?v=2R5cjpi9Fzw

You may add your notes here:

1. There are many cases where one can use 'const', but variables, class member functions, and function arguments are three instances where I would use it.
2. In the context of functions, the inline keyword suggests to the compiler that it should attempt to generate the function's machine code in-line at each point where the function is called, rather than generating a traditional function call. In other words, it's a hint to replace the function call with the actual code of the function if the compiler deems it beneficial. This can improve performance by eliminating the overhead of a function call. Moreover, there's no need for pushing or popping data to/from the stack for arguments or return values.
3. One of the primary advantages of header-only libraries is the ease of integration into a project. Users can simply #include the necessary header files without having to worry about linking against a precompiled library. This is especially beneficial for projects that span diverse build systems or platforms.
4. If you overload the << operator for a mat4 type, you would provide an easy way to print the matrix using I/O streams. This could be particularly useful for debugging purposes. The GLM library does provide overloads for the << operator when you include the appropriate GLM headers.
5. Yes, the inherent imprecision of floating-point arithmetic and representation can lead to unexpected results.
Epsilon comparison can mitigate this. It involves comparing the absolute difference of two floating-point numbers against a small value (epsilon). If the difference is smaller than epsilon, the numbers can be treated as approximately equal.
Floating-point numbers in computers are represented in binary format, adhering to the IEEE 754 standard for most systems. Because of this representation, not all decimal numbers can be precisely represented as binary floating-point numbers. Moreover, operations on floating-point numbers can introduce small rounding errors. Due to the order of operations and the associated rounding, even if two sequences of operations are mathematically equivalent, they might not yield the exact same floating-point result.
6. A template allows functions and classes to operate with generic types, similar to Java’s generics. This means that a single function or class can work with different data types without needing to be explicitly rewritten for each one.
The GLM library makes extensive use of templates. GLM provides types and functions tailored for graphics and mathematics. Many of its types, such as vectors and matrices, are templated based on both the type of components (e.g., float, double, int) and their dimensions (e.g., 2, 3, 4 for vectors). This enables GLM to offer a wide range of types, like ‘glm::vec2’, ‘glm::vec3’, ‘glm::vec4’, ‘glm::mat4’, etc., without having to write redundant code for each specific type and size.
7. One of the primary design goals of GLM is to mimic as closely as possible the functions and behaviors present in GLSL. This allows developers, especially those familiar with GLSL, to achieve a consistent and predictable experience when transitioning between CPU-side code (using GLM) and GPU-side code (using GLSL).
8. No, I have used pointers in C several times, but implementing them is always tricky.
