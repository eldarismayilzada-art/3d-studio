package com.bankrespublika.bankrespublika.models;

import jakarta.persistence.*;
import java.time.LocalDate;

@Entity
@Table(name = "transactions")
public class Transactions {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private int id;

    @Column(name = "from_account_id", nullable = false)
    private int fromAccountId;

    @Column(name = "to_account_id", nullable = false)
    private int toAccountId;

    @Column(name = "amount", nullable = false)
    private int amount;

    @Column(name = "transaction_type", nullable = false)
    private String transactionType;  // "TRANSFER", "PAYMENT", etc

    @Column(name = "status", nullable = false)
    private String status;  // "SUCCESS", "FAILED", "PENDING"

    @Column(name = "description")
    private String description;

    @Column(name = "timestamp", nullable = false)
    private LocalDate timestamp;

    // ===== CONSTRUCTORS =====
    public Transactions(int fromID, int toID, int amount,
                        String transfer,
                        String success,
                        String description,
                        LocalDate now) {

        this.fromAccountId = fromID;
        this.toAccountId = toID;
        this.amount = amount;
        this.transactionType = transfer;
        this.status = success;
        this.description = description;
        this.timestamp = now;
    }
    public Transactions() {
    }

    public Transactions(int fromAccountId, int toAccountId, int amount,
                        String transactionType, String status, String description) {
        this.fromAccountId = fromAccountId;
        this.toAccountId = toAccountId;
        this.amount = amount;
        this.transactionType = transactionType;
        this.status = status;
        this.description = description;
        this.timestamp = LocalDate.now();
    }

    // ===== GETTERS AND SETTERS =====
    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getFromAccountId() {
        return fromAccountId;
    }

    public void setFromAccountId(int fromAccountId) {
        this.fromAccountId = fromAccountId;
    }

    public int getToAccountId() {
        return toAccountId;
    }

    public void setToAccountId(int toAccountId) {
        this.toAccountId = toAccountId;
    }

    public int getAmount() {
        return amount;
    }

    public void setAmount(int amount) {
        this.amount = amount;
    }

    public String getTransactionType() {
        return transactionType;
    }

    public void setTransactionType(String transactionType) {
        this.transactionType = transactionType;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public LocalDate getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(LocalDate timestamp) {
        this.timestamp = timestamp;
    }

    @Override
    public String toString() {
        return "Transactions{" +
                "id=" + id +
                ", fromAccountId=" + fromAccountId +
                ", toAccountId=" + toAccountId +
                ", amount=" + amount +
                ", transactionType='" + transactionType + '\'' +
                ", status='" + status + '\'' +
                ", description='" + description + '\'' +
                ", timestamp=" + timestamp +
                '}';
    }
}