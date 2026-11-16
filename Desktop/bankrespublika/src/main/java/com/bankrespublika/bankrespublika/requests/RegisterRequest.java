package com.bankrespublika.bankrespublika.requests;
import jakarta.validation.constraints.*;

public class RegisterRequest {
    @Email(message = "Email must be valid")
    @NotEmpty(message="Emeail is required")
    private String email;
    @NotEmpty(message = "Password cannot be empty")
    @Min(value = 8 , message = "Password should be at least 8")
    private String password;
    @NotEmpty(message= "Full Name must be not empty")
    private String full_name;
    @NotEmpty(message = "Cannot be mot empty")
    @Min(value = 10, message="Should be at least 10 inegers")
    private String phonenumber;
    public RegisterRequest(){}
    public String getEmail(){return email;}

    public String getPassword_hash() {
        return password;
    }

    public String getFull_name() {
        return full_name;
    }

    public String getPhonenumber() {
        return phonenumber;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public void setPassword_hash(String password_hash) {
        this.password = password_hash;
    }

    public void setFull_name(String full_name) {
        this.full_name = full_name;
    }

    public void setPhonenumber(String phonenumber) {
        this.phonenumber = phonenumber;
    }
}
