package com.bankrespublika.bankrespublika.models;
import com.fasterxml.jackson.annotation.JsonBackReference;
import jakarta.persistence.*;
import java.time.LocalDate;

@Entity
@Table(name = "accounts")
public class Accounts {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private int id;

    @ManyToOne
    @JoinColumn(name = "user_id")
    @JsonBackReference

    private Users user;

    private int account_number;
    private String account_type;
    private int balance;
    private String status;
    private String description;
    private LocalDate timestamp;

    public Accounts() {
    }

    public Accounts(Users user, String account_type, int balance) {
        this.user = user;
        this.account_type = account_type;
        this.balance = balance;
        this.account_number = generateAccountNumber();
        this.status = "ACTIVE";
        this.description = "New account";
        this.timestamp = LocalDate.now();
    }

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