package com.bankrespublika.bankrespublika.RESTAPIConrtollers;

import com.bankrespublika.bankrespublika.dto.AccountDto;
import com.bankrespublika.bankrespublika.interfaces.AccountRepository;
import com.bankrespublika.bankrespublika.interfaces.UserRepository;
import com.bankrespublika.bankrespublika.models.Accounts;
import com.bankrespublika.bankrespublika.models.Users;
import com.bankrespublika.bankrespublika.requests.CreateAccountRequest;
import jakarta.validation.Valid;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.time.LocalDate;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Optional;

@RestController
@RequestMapping("/api/accounts")
@CrossOrigin(origins = "http://localhost:3000")
public class AccountController {

    @Autowired
    private AccountRepository accountRepository;

    @Autowired
    private UserRepository userRepository;

    @PostMapping
    public ResponseEntity<?> createAccount(@Valid @RequestBody CreateAccountRequest request) {

        Optional<Users> optionalUser = userRepository.findById(request.getUserId());

        if (optionalUser.isEmpty()) {
            return ResponseEntity.status(HttpStatus.NOT_FOUND)
                    .body("User not found");
        }

        Users user = optionalUser.get();

        Accounts account = new Accounts();

        account.setUser(user);
        account.setAccount_type(request.getAccount_type());
        account.setBalance(request.getBalance());
        account.setAccount_number(generateAccountNumber());
        account.setStatus("ACTIVE");
        account.setDescription("New account");
        account.setTimestamp(LocalDate.now());

        Accounts savedAccount = accountRepository.save(account);

        Map<String, Object> response = new HashMap<>();
        response.put("message", "Account created successfully");
        response.put("accountId", savedAccount.getId());
        response.put("accountNumber", savedAccount.getAccount_number());
        response.put("accountType", savedAccount.getAccount_type());
        response.put("balance", savedAccount.getBalance());

        return ResponseEntity.status(HttpStatus.CREATED).body(response);
    }

    @GetMapping("/user/{userId}")
    public ResponseEntity<?> getAccountsByUser(@PathVariable int userId) {

        Optional<Users> optionalUser = userRepository.findById(userId);

        if (optionalUser.isEmpty()) {
            return ResponseEntity.status(HttpStatus.NOT_FOUND)
                    .body("User not found");
        }

        List<Accounts> accounts = accountRepository.findByUserId(optionalUser.get().getId());

        AccountDto accountDto = new AccountDto();
        accountDto.setUser(optionalUser.get());
        accountDto.setAccount_type(accounts.get(0).getAccount_type());
        accountDto.setAccount_number(accounts.get(0).getAccount_number());
        accountDto.setId(accounts.get(0).getId());
        accountDto.setBalance(accounts.get(0).getBalance());
        accountDto.setStatus(accounts.get(0).getStatus());
        return ResponseEntity.ok(accountDto);
    }

    @GetMapping("/{id}")
    public ResponseEntity<?> getAccountById(@PathVariable int id) {

        Optional<Accounts> account = accountRepository.findById(id);

        if (account.isPresent()) {
            return ResponseEntity.ok(account.get());
        }

        return ResponseEntity.status(HttpStatus.NOT_FOUND)
                .body("Account not found");
    }

    @GetMapping("/{id}/balance")
    public ResponseEntity<?> getBalance(@PathVariable int id) {

        Optional<Accounts> account = accountRepository.findById(id);

        if (account.isEmpty()) {
            return ResponseEntity.status(HttpStatus.NOT_FOUND)
                    .body("Account not found");
        }

        Map<String, Object> response = new HashMap<>();
        response.put("accountId", account.get().getId());
        response.put("accountNumber", account.get().getAccount_number());
        response.put("balance", account.get().getBalance());

        return ResponseEntity.ok(response);
    }

    private int generateAccountNumber() {
        return (int) (System.currentTimeMillis() % 100000000);
    }
}