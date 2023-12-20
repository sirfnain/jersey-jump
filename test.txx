syntax = "proto3";

option java_multiple_files = true;
option java_package = "com.example.model";
option java_outer_classname = "ApiResponse";

message ApiResponse {
  // Define your message fields here
  string message = 1;
}


pom.xml
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <groupId>com.example</groupId>
    <artifactId>protobuf-token-oracle-example</artifactId>
    <version>1.0-SNAPSHOT</version>
    <packaging>jar</packaging>

    <properties>
        <java.version>11</java.version>
        <spring.boot.version>2.6.5</spring.boot.version>
        <protobuf.version>3.18.1</protobuf.version>
        <logback.version>1.2.3</logback.version>
        <oracle.version>19.11</oracle.version>
    </properties>

    <dependencies>
        <!-- Spring Boot Starter for Web -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
            <version>${spring.boot.version}</version>
        </dependency>

        <!-- Spring Boot Starter for JDBC -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-jdbc</artifactId>
            <version>${spring.boot.version}</version>
        </dependency>

        <!-- Protobuf Compiler -->
        <dependency>
            <groupId>com.google.protobuf</groupId>
            <artifactId>protobuf-java</artifactId>
            <version>${protobuf.version}</version>
        </dependency>

        <!-- Oracle JDBC Driver -->
        <dependency>
            <groupId>com.oracle.database.jdbc</groupId>
            <artifactId>ojdbc8</artifactId>
            <version>${oracle.version}</version>
            <scope>runtime</scope>
        </dependency>

        <!-- Logback for logging -->
        <dependency>
            <groupId>ch.qos.logback</groupId>
            <artifactId>logback-classic</artifactId>
            <version>${logback.version}</version>
        </dependency>

        <!-- Spring Boot Starter Test -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-test</artifactId>
            <scope>test</scope>
        </dependency>
    </dependencies>

    <build>
        <plugins>
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
            </plugin>
            <plugin>
                <groupId>org.xolstice.maven.plugins</groupId>
                <artifactId>protobuf-maven-plugin</artifactId>
                <version>0.6.1</version>
                <configuration>
                    <protocArtifact>com.google.protobuf:protoc:${protobuf.version}:exe:${os.detected.classifier}</protocArtifact>
                </configuration>
                <executions>
                    <execution>
                        <goals>
                            <goal>compile</goal>
                        </goals>
                    </execution>
                </executions>
            </plugin>
        </plugins>
    </build>
</project>

application.properties

auth.server.url=https://your-auth-server.com
auth.client.p12-path=/path/to/your/certificate.p12
auth.client.p12-password=your-certificate-password
spring.datasource.url=jdbc:oracle:thin:@your-oracle-host:1521:your-database
spring.datasource.username=your-database-username
spring.datasource.password=your-database-password
spring.datasource.driver-class-name=oracle.jdbc.OracleDriver
external.api.url=https://your-api-endpoint
retry.max-attempts=3
retry.delay-ms=1000


package com.example.service;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
import org.springframework.util.ResourceUtils;
import org.springframework.web.client.RestTemplate;

import java.io.FileInputStream;
import java.security.KeyStore;
import java.security.PrivateKey;
import java.security.cert.Certificate;

@Service
public class TokenService {

    private final Logger logger = LoggerFactory.getLogger(TokenService.class);
    private final RestTemplate restTemplate;
    
    @Value("${auth.server.url}")
    private String authServerUrl;

    @Value("${auth.client.p12-path}")
    private String p12Path;

    @Value("${auth.client.p12-password}")
    private String p12Password;

    @Value("${retry.max-attempts}")
    private int maxRetryAttempts;

    @Value("${retry.delay-ms}")
    private long retryDelay;

    public TokenService(RestTemplate restTemplate) {
        this.restTemplate = restTemplate;
    }

    public String getTokenWithRetry() {
        int attempts = 0;
        while (attempts < maxRetryAttempts) {
            try {
                return getToken();
            } catch (Exception e) {
                logger.error("Error obtaining token (Attempt {}): {}", attempts + 1, e.getMessage());
                attempts++;
                if (attempts < maxRetryAttempts) {
                    // Apply delay before the next retry
                    try {
                        Thread.sleep(retryDelay);
                    } catch (InterruptedException ignored) {
                        Thread.currentThread().interrupt();
                    }
                }
            }
        }
        throw new RuntimeException("Failed to obtain token after multiple attempts");
    }

    private String getToken() throws Exception {
        PrivateKey privateKey = loadP12Certificate();
        // Use the private key to obtain the token from the authentication server
        // Customize this based on your actual token endpoint and response format
        // For simplicity, assume the response is a String (you may need to adapt this based on the actual response format)
        // String token = restTemplate.postForObject(authServerUrl, privateKey, String.class);
        String token = "your-token-response"; // Replace with the actual logic
        // Log the successful token retrieval
        logger.info("Token obtained successfully");
        return token;
    }

    private PrivateKey loadP12Certificate() throws Exception {
        KeyStore keyStore = KeyStore.getInstance("PKCS12");
        try (FileInputStream fis = new FileInputStream(ResourceUtils.getFile(p12Path))) {
            keyStore.load(fis, p12Password.toCharArray());
        }

        // Assuming there is only one entry in the keystore
        String alias = keyStore.aliases().nextElement();
        Certificate cert = keyStore.getCertificate(alias);

        if (cert == null) {
            throw new RuntimeException("No certificate found in the keystore");
        }

        // Assuming the certificate entry contains a private key
        return (PrivateKey) keyStore.getKey(alias, p12Password.toCharArray());
    }
}



package com.example.service;

import com.example.model.ApiResponse;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.CommandLineRunner;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Service;
import org.springframework.web.client.RestTemplate;

@Service
public class ApiService implements CommandLineRunner {

    private final RestTemplate restTemplate;
    private final JdbcTemplate jdbcTemplate;
    private final TokenService tokenService;
    private final Logger logger = LoggerFactory.getLogger(ApiService.class);

    @Value("${external.api.url}")
    private String externalApiUrl;

    @Autowired
    public ApiService(RestTemplate restTemplate, JdbcTemplate jdbcTemplate, TokenService tokenService) {
        this.restTemplate = restTemplate;
        this.jdbcTemplate = jdbcTemplate;
        this.tokenService = tokenService;
    }

    @Override
    public void run(String... args) throws Exception {
        ApiResponse apiResponse = fetchData();
        // Additional initialization logic if needed
        logger.info("Application initialized with ApiResponse: {}", apiResponse);
    }

    public ApiResponse fetchData() {
        String token = tokenService.getTokenWithRetry();
        String response = callExternalApi(token);
        ApiResponse apiResponse = transformResponse(response);
        saveResponseInDatabase(apiResponse);
        return apiResponse;
    }

    protected String callExternalApi(String token) {
        try {
            // Use the token to call the external API and retrieve data
            // Customize this based on the actual API endpoint and response format
            // For simplicity, assume the response is a String
            return restTemplate.getForObject(externalApiUrl, String.class);
        } catch (Exception e) {
            logger.error("Error calling external API: {}", e.getMessage());
            throw e;
        }
    }

    protected ApiResponse transformResponse(String response) {
        try {
            // Transform the response to ApiResponse protobuf message
            ApiResponse.Builder builder = ApiResponse.newBuilder();
            builder.setMessage(response);
            return builder.build();
        } catch (Exception e) {
            logger.error("Error transforming response: {}", e.getMessage());
            throw e;
        }
    }

    protected void saveResponseInDatabase(ApiResponse apiResponse) {
        try {
            // Save the ApiResponse in the database
            // Customize this based on your actual database structure and requirements
            // For simplicity, assume the database table has a column 'message'
            // You may need to create the table and handle exceptions properly
            jdbcTemplate.update("INSERT INTO responses (message) VALUES (?)", apiResponse.getMessage());
        } catch (Exception e) {
            logger.error("Error saving response in database: {}", e.getMessage());
            throw e;
        }
    }
}




package com.example.service;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class DatabaseService {

    private final JdbcTemplate jdbcTemplate;
    private final Logger logger = LoggerFactory.getLogger(DatabaseService.class);

    @Autowired
    public DatabaseService(JdbcTemplate jdbcTemplate) {
        this.jdbcTemplate = jdbcTemplate;
    }

    public List<String> fetchDataFromDatabase() {
        try {
            // Execute a SQL query to fetch data from the database
            // Customize this query based on your actual database schema and requirements
            String sqlQuery = "SELECT column_name FROM your_table_name";
            return jdbcTemplate.queryForList(sqlQuery, String.class);
        } catch (Exception e) {
            logger.error("Error fetching data from database: {}", e.getMessage());
            throw e;
        }
    }
}

src
├── main
│   ├── java
│   │   └── com
│   │       └── example
│   │           ├── model
│   │           │   └── ApiResponse.java
│   │           ├── service
│   │           │   ├── ApiService.java
│   │           │   ├── DatabaseService.java
│   │           │   └── TokenService.java
│   │           └── YourApplication.java
│   ├── proto
│   │   └── ApiResponse.proto
│   └── resources
│       └── application.properties
└── test
    └── java
        └── com
            └── example
                └── service
                    ├── ApiServiceTest.java
                    └── DatabaseServiceTest.java

package com.example.service;

import com.example.model.ApiResponse;
import org.junit.jupiter.api.Test;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.Mockito;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.web.client.RestTemplate;

import static org.junit.jupiter.api.Assertions.assertEquals;

@SpringBootTest
class ApiServiceTest {

    @Mock
    private RestTemplate restTemplate;

    @Mock
    private DatabaseService databaseService;

    @Mock
    private TokenService tokenService;

    @InjectMocks
    private ApiService apiService;

    @Test
    void testFetchData() {
        // Mock external API response
        String mockExternalApiResponse = "Mocked external API response";
        Mockito.when(restTemplate.getForObject(Mockito.anyString(), Mockito.eq(String.class)))
               .thenReturn(mockExternalApiResponse);

        // Mock token retrieval
        String mockToken = "MockedToken";
        Mockito.when(tokenService.getTokenWithRetry()).thenReturn(mockToken);

        // Mock database response
        Mockito.when(databaseService.fetchDataFromDatabase()).thenReturn(List.of("data1", "data2"));

        // Call the method under test
        ApiResponse result = apiService.fetchData();

        // Assertions
        assertEquals(mockExternalApiResponse, result.getMessage());
        // Add more assertions based on the actual behavior of your service
    }
}


package com.example.service;

import org.junit.jupiter.api.Test;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.Mockito;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.jdbc.core.JdbcTemplate;

import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;

@SpringBootTest
class DatabaseServiceTest {

    @Mock
    private JdbcTemplate jdbcTemplate;

    @InjectMocks
    private DatabaseService databaseService;

    @Test
    void testFetchDataFromDatabase() {
        // Mock database response
        List<String> mockDatabaseData = Arrays.asList("MockedData1", "MockedData2");
        Mockito.when(jdbcTemplate.queryForList(Mockito.anyString(), Mockito.eq(String.class)))
               .thenReturn(mockDatabaseData);

        // Call the method under test
        List<String> result = databaseService.fetchDataFromDatabase();

        // Assertions
        assertEquals(mockDatabaseData, result);
        // Add more assertions based on the actual behavior of your service
    }
}
