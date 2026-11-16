package com.bankrespublika.bankrespublika.RESTAPIConrtollers;

import com.bankrespublika.bankrespublika.interfaces.UserRepository;
import com.bankrespublika.bankrespublika.models.Users;
import com.bankrespublika.bankrespublika.requests.ChangePasswordRequest;
import com.bankrespublika.bankrespublika.requests.CreateUserRequest;
import com.bankrespublika.bankrespublika.requests.UpdatedUserRequest;

import jakarta.validation.Valid;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.time.LocalDate;
import java.util.HashMap;
import java.util.Map;
import java.util.Optional;

@RestController
@RequestMapping("/api/profile")
@CrossOrigin(origins = "http://localhost:3000")
public class UserController {

    @Autowired
    private UserRepository userRepository;

    // ================= REGISTER =================

    @PostMapping("/register")
    public ResponseEntity<?> register(@RequestBody CreateUserRequest request) {

        // Check if email already exists
        if (userRepository.findByEmail(request.getEmail()).isPresent()) {
            return ResponseEntity.badRequest().body("Email already exists");
        }

        Users user = new Users();

        user.setEmail(request.getEmail());
        user.setPassword_hash(request.getPassword_hash());
        user.setFull_name(request.getFull_name());
        user.setPhonenumber(request.getPhonenumber());

        user.setStatus("ACTIVE");
        user.setCreatedAt(LocalDate.now());
        user.setUpdated_at(LocalDate.now());

        Users savedUser = userRepository.save(user);

        return ResponseEntity.status(HttpStatus.CREATED).body(savedUser);
    }

    // ================= GET PROFILE =================

    @GetMapping
    public ResponseEntity<?> getProfile() {

        Optional<Users> user = userRepository.findById(1);

        if (user.isEmpty()) {
            return ResponseEntity.notFound().build();
        }

        Users u = user.get();

        Map<String, Object> response = new HashMap<>();
        response.put("id", u.getId());
        response.put("email", u.getEmail());
        response.put("fullName", u.getFull_name());
        response.put("phone", u.getPhonenumber());
        response.put("status", u.getStatus());

        return ResponseEntity.ok(response);
    }

    // ================= UPDATE PROFILE =================

    @PutMapping
    public ResponseEntity<?> updateProfile(@Valid @RequestBody UpdatedUserRequest request) {

        Optional<Users> user = userRepository.findById(1);

        if (user.isEmpty()) {
            return ResponseEntity.notFound().build();
        }

        Users updatedUser = user.get();

        updatedUser.setFull_name(request.getFull_name());
        updatedUser.setPhonenumber(request.getPhonenumber());
        updatedUser.setUpdated_at(LocalDate.now());

        userRepository.save(updatedUser);

        return ResponseEntity.ok(updatedUser);
    }

    // ================= CHANGE PASSWORD =================

    @PostMapping("/change-password")
    public ResponseEntity<?> changePassword(@Valid @RequestBody ChangePasswordRequest request) {

        Optional<Users> user = userRepository.findById(1);

        if (user.isEmpty()) {
            return ResponseEntity.notFound().build();
        }

        Users currentUser = user.get();

        if (!currentUser.getPassword_hash().equals(request.getCurrentPassword())) {
            return ResponseEntity.badRequest().body("Current password is incorrect");
        }

        if (!request.getNewPassword().equals(request.getConfirmPassword())) {
            return ResponseEntity.badRequest().body("Passwords do not match");
        }

        currentUser.setPassword_hash(request.getNewPassword());
        currentUser.setUpdated_at(LocalDate.now());

        userRepository.save(currentUser);

        return ResponseEntity.ok("Password changed successfully");
    }
}