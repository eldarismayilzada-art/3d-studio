package com.bankrespublika.bankrespublika.interfaces;

import com.bankrespublika.bankrespublika.models.Accounts;
import com.bankrespublika.bankrespublika.models.Users;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface AccountRepository extends JpaRepository<Accounts, Integer> {

    List<Accounts> findByUserId(Integer userId);
}