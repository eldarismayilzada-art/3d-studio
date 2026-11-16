package com.bankrespublika.bankrespublika.dto;

import com.bankrespublika.bankrespublika.models.Users;

import java.time.LocalDate;

public class AccountDto {
    private int id;


    private Users user;

    private int account_number;
    private String account_type;
    private int balance;
    private String status;
    private String description;
    private LocalDate timestamp;


    private int generateAccountNumber() {
        return (int) (System.currentTimeMillis() % 100000000);
    }

    // ================= GETTERS =================

    public int getId() {
        return id;
    }

    public Users getUser() {
        return user;
    }

    public int getAccount_number() {
        return account_number;
    }

    public String getAccount_type() {
        return account_type;
    }

    public int getBalance() {
        return balance;
    }

    public String getStatus() {
        return status;
    }

    public String getDescription() {
        return description;
    }

    public LocalDate getTimestamp() {
        return timestamp;
    }

    // ================= SETTERS =================

    public void setId(int id) {
        this.id = id;
    }

    public void setUser(Users user) {
        this.user = user;
    }

    public void setAccount_number(int account_number) {
        this.account_number = account_number;
    }

    public void setAccount_type(String account_type) {
        this.account_type = account_type;
    }

    public void setBalance(int balance) {
        this.balance = balance;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public void setTimestamp(LocalDate timestamp) {
        this.timestamp = timestamp;
    }
}
