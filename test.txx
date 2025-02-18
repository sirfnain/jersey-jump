<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <groupId>com.example</groupId>
    <artifactId>spring-grpc-jdk21</artifactId>
    <version>1.0.0</version>
    <packaging>jar</packaging>

    <properties>
        <java.version>21</java.version>
        <spring-boot.version>3.2.0</spring-boot.version> <!-- Latest Spring Boot version -->
        <grpc.version>1.58.0</grpc.version> <!-- Latest gRPC version -->
        <protobuf.version>3.24.4</protobuf.version> <!-- Latest Protobuf version -->
        <guava.version>32.1.3-jre</guava.version> <!-- Latest Guava version -->
        <commons-lang.version>3.13.0</commons-lang.version> <!-- Latest Commons Lang version -->
        <commons-io.version>2.15.1</commons-io.version> <!-- Latest Commons IO version -->
        <jaxb2-maven-plugin.version>3.1.0</jaxb2-maven-plugin.version> <!-- JAXB2 Maven Plugin version -->
    </properties>

    <dependencies>
        <!-- Spring Boot Starter Dependencies -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
            <version>${spring-boot.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-jdbc</artifactId>
            <version>${spring-boot.version}</version>
        </dependency>

        <!-- gRPC and Protobuf Dependencies -->
        <dependency>
            <groupId>io.grpc</groupId>
            <artifactId>grpc-netty-shaded</artifactId>
            <version>${grpc.version}</version>
        </dependency>
        <dependency>
            <groupId>io.grpc</groupId>
            <artifactId>grpc-protobuf</artifactId>
            <version>${grpc.version}</version>
        </dependency>
        <dependency>
            <groupId>io.grpc</groupId>
            <artifactId>grpc-stub</artifactId>
            <version>${grpc.version}</version>
        </dependency>
        <dependency>
            <groupId>com.google.protobuf</groupId>
            <artifactId>protobuf-java</artifactId>
            <version>${protobuf.version}</version>
        </dependency>

        <!-- Logback for Logging -->
        <dependency>
            <groupId>ch.qos.logback</groupId>
            <artifactId>logback-classic</artifactId>
            <version>1.4.11</version> <!-- Latest Logback version -->
        </dependency>

        <!-- Jakarta XML Binding (JAXB) -->
        <dependency>
            <groupId>jakarta.xml.bind</groupId>
            <artifactId>jakarta.xml.bind-api</artifactId>
            <version>4.0.0</version> <!-- Latest Jakarta XML Binding version -->
        </dependency>
        <dependency>
            <groupId>org.glassfish.jaxb</groupId>
            <artifactId>jaxb-runtime</artifactId>
            <version>4.0.0</version> <!-- Latest JAXB Runtime version -->
        </dependency>

        <!-- Guava -->
        <dependency>
            <groupId>com.google.guava</groupId>
            <artifactId>guava</artifactId>
            <version>${guava.version}</version>
        </dependency>

        <!-- Apache Commons -->
        <dependency>
            <groupId>org.apache.commons</groupId>
            <artifactId>commons-lang3</artifactId>
            <version>${commons-lang.version}</version>
        </dependency>
        <dependency>
            <groupId>commons-io</groupId>
            <artifactId>commons-io</artifactId>
            <version>${commons-io.version}</version>
        </dependency>
    </dependencies>

    <build>
        <plugins>
            <!-- Maven Compiler Plugin for JDK 21 -->
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
                <version>3.11.0</version>
                <configuration>
                    <source>${java.version}</source>
                    <target>${java.version}</target>
                </configuration>
            </plugin>

            <!-- Spring Boot Maven Plugin -->
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
                <version>${spring-boot.version}</version>
                <configuration>
                    <mainClass>com.example.Application</mainClass> <!-- Replace with your main class -->
                </configuration>
            </plugin>

            <!-- Protobuf Maven Plugin for gRPC -->
            <plugin>
                <groupId>org.xolstice.maven.plugins</groupId>
                <artifactId>protobuf-maven-plugin</artifactId>
                <version>0.6.1</version>
                <configuration>
                    <protocArtifact>com.google.protobuf:protoc:${protobuf.version}:exe:${os.detected.classifier}</protocArtifact>
                    <pluginId>grpc-java</pluginId>
                    <pluginArtifact>io.grpc:protoc-gen-grpc-java:${grpc.version}:exe:${os.detected.classifier}</pluginArtifact>
                </configuration>
                <executions>
                    <execution>
                        <goals>
                            <goal>compile</goal>
                            <goal>compile-custom</goal>
                        </goals>
                    </execution>
                </executions>
            </plugin>

            <!-- JAXB2 Maven Plugin for XSD to Java Classes -->
            <plugin>
                <groupId>org.codehaus.mojo</groupId>
                <artifactId>jaxb2-maven-plugin</artifactId>
                <version>${jaxb2-maven-plugin.version}</version>
                <executions>
                    <execution>
                        <goals>
                            <goal>xjc</goal> <!-- Generates Java classes from XSD -->
                        </goals>
                    </execution>
                </executions>
                <configuration>
                    <sources>
                        <source>src/main/xsd</source> <!-- Directory containing your XSD files -->
                    </sources>
                    <packageName>com.example.generated</packageName> <!-- Package for generated classes -->
                    <outputDirectory>${project.build.directory}/generated-sources/jaxb</outputDirectory>
                </configuration>
            </plugin>
        </plugins>
    </build>
</project>
