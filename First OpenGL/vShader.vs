	    #version 330 core
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec3 aNormal;
		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

		uniform vec3 lightColor;
        uniform vec3 lightPos;
        uniform vec3 viewPos;

		out vec3 Result;

		void main()
		{
		vec3 Normal = mat3(transpose(inverse(model))) * aNormal;
		vec3 FragPos = vec3(model * vec4(aPos, 1.0));
		float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * lightColor;

        vec3 lightDir = normalize(lightPos - FragPos);
        vec3 norm = normalize(Normal);
       float diff = max(dot(norm, lightDir), 0.0);
       vec3 diffuse = diff * lightColor;


    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    Result = ambient + diffuse + specular;
		gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
		}