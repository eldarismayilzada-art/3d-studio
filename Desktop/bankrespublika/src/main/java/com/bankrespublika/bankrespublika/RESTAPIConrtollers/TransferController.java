package com.bankrespublika.bankrespublika.RESTAPIConrtollers;

import com.bankrespublika.bankrespublika.interfaces.AccountRepository;
import com.bankrespublika.bankrespublika.models.Accounts;
import com.bankrespublika.bankrespublika.models.Transactions;
import com.bankrespublika.bankrespublika.requests.TransferRequest;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;

import java.time.LocalDate;
import java.util.Optional;

public class TransferController {
    @Autowired
    private AccountRepository accountRepository;
    @PostMapping
    public ResponseEntity<?> tranfer(@RequestBody TransferRequest request){
        int fromID = request.getFromAccountID();
        int toID = request.getToAccountID();
        int amount = request.getAmount();
        Optional<Accounts> fromAccount = accountRepository.findById(fromID);
        Optional<Accounts> toAccount = accountRepository.findById(toID);
        if(!fromAccount.isPresent()||!toAccount.isPresent()){
            return ResponseEntity.badRequest().body("Account not found");

        }

        Accounts from = fromAccount.get();
        Accounts to = toAccount.get();
        from.setBalance(from.getBalance()-amount);
        to.setBalance((to.getBalance() + amount));
        accountRepository.save(from);
        accountRepository.save(to);

        Transactions transaction = new Transactions(
                fromID,
                toID,
                amount,
                "TRANSFER",
                "SUCCESS",
                request.getDescription(),
                LocalDate.now()
        );


        return ResponseEntity.ok("Transfer Succesfull");
    }


}
