package com.bankrespublika.bankrespublika.interfaces;

import com.bankrespublika.bankrespublika.models.Accounts;
import com.bankrespublika.bankrespublika.models.Users;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;
import java.util.Optional;

public interface UserRepository extends JpaRepository<Users, Integer> {

    Optional<Users> findByEmail(String email);

}