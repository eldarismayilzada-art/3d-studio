package com.bankrespublika.bankrespublika.models;

import com.fasterxml.jackson.annotation.JsonBackReference;
import com.fasterxml.jackson.annotation.JsonIgnore;
import jakarta.persistence.*;
import java.time.LocalDate;
import java.util.List;

@Entity
@Table(name = "users")
public class Users {

    @OneToMany(mappedBy = "user", fetch = FetchType.LAZY)
    @JsonBackReference
    private List<Accounts> accounts;
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private int id;

    private String email;
    private String password_hash;
    private String full_name;
    private String phonenumber;
    private String status;
    private LocalDate createdAt;
    private LocalDate updated_at;

    public Users() {
    }

    public Users(int id, String email, String password_hash, String full_name,
                 String phonenumber, String status,
                 LocalDate createdAt, LocalDate updated_at) {

        this.id = id;
        this.email = email;
        this.password_hash = password_hash;
        this.full_name = full_name;
        this.phonenumber = phonenumber;
        this.status = status;
        this.createdAt = createdAt;
        this.updated_at = updated_at;
    }

    // ================= GETTERS =================

    public int getId() {
        return id;
    }

    public String getEmail() {
        return email;
    }

    public String getPassword_hash() {
        return password_hash;
    }

    public String getFull_name() {
        return full_name;
    }

    public String getPhonenumber() {
        return phonenumber;
    }

    public String getStatus() {
        return status;
    }

    public LocalDate getCreatedAt() {
        return createdAt;
    }

    public LocalDate getUpdated_at() {
        return updated_at;
    }

    // ================= SETTERS =================

    public void setId(int id) {
        this.id = id;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public void setPassword_hash(String password_hash) {
        this.password_hash = password_hash;
    }

    public void setFull_name(String full_name) {
        this.full_name = full_name;
    }

    public void setPhonenumber(String phonenumber) {
        this.phonenumber = phonenumber;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public void setCreatedAt(LocalDate createdAt) {
        this.createdAt = createdAt;
    }

    public void setUpdated_at(LocalDate updated_at) {
        this.updated_at = updated_at;
    }
    public List<Accounts> getAccounts() {
        return accounts;
    }

    public void setAccounts(List<Accounts> accounts) {
        this.accounts = accounts;
    }
}