    package com.bankrespublika.bankrespublika.models;

    import jakarta.persistence.*;

    @Entity
    @Table(name="payees")

    public class Payees {
        @Id
                @GeneratedValue(strategy = GenerationType.IDENTITY)
        int id;
        int user_id;
        String payee_name;
        private int payeename;
        int banc_code;
        boolean status;
        public Payees() {
        }
        public Payees(int id, int user_id, String payee_name, int payeename, int banc_code, boolean status) {
            this.id = id;
            this.user_id = user_id;
            this.payee_name = payee_name;
            this.payeename = payeename;
            this.banc_code = banc_code;
            this.status = status;
        }

        public int getId() {
            return id;
        }

        public int getUser_id() {
            return user_id;
        }

        public String getPayee_name() {
            return payee_name;
        }

        public int getPayeename() {
            return payeename;
        }

        public int getBanc_code() {
            return banc_code;
        }

        public boolean isStatus() {
            return status;
        }
    }
