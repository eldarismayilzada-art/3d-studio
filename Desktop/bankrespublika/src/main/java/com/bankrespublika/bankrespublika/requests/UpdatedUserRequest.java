package com.bankrespublika.bankrespublika.requests;

import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;

public class UpdatedUserRequest {

    @NotBlank(message = "Full name is required")
    private String full_name;

    @NotBlank(message = "Phone is required")
    @Size(min = 10, max = 15, message = "Phone must be 10-15 characters")
    private String phonenumber;

    // ===== CONSTRUCTORS =====
    public UpdatedUserRequest() {}

    public UpdatedUserRequest(String full_name, String phonenumber) {
        this.full_name = full_name;
        this.phonenumber = phonenumber;
    }

    // ===== GETTERS AND SETTERS =====
    public String getFull_name() {
        return full_name;
    }

    public void setFull_name(String full_name) {
        this.full_name = full_name;
    }

    public String getPhonenumber() {
        return phonenumber;
    }

    public void setPhonenumber(String phonenumber) {
        this.phonenumber = phonenumber;
    }
}