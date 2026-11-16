package com.bankrespublika.bankrespublika.interfaces;

import com.bankrespublika.bankrespublika.models.Transactions;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface TransactionRepository extends JpaRepository<Transactions, Integer> {
    List<Transactions> findByFromAccountId(int fromAccountId);
    List<Transactions> findByToAccountId(int toAccountId);
}